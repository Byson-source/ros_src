#include <ros/ros.h>

#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#include <actionlib/server/simple_action_server.h>
#include <cpp/RtabmapReprocessAction.h>
#include <stdio.h>
#include <string>

#include <rtabmap/core/Rtabmap.h>
#include <rtabmap/core/DBDriver.h>
#include <rtabmap/core/DBReader.h>
#include <rtabmap/core/OccupancyGrid.h>
#include <rtabmap/core/Graph.h>
#include <rtabmap/core/Memory.h>
#include <rtabmap/core/CameraThread.h>
#include <rtabmap/utilite/UFile.h>
#include <rtabmap/utilite/UDirectory.h>
#include <rtabmap/utilite/UTimer.h>
#include <rtabmap/utilite/UStl.h>
#include <rtabmap/utilite/UMath.h>
#include <stdlib.h>
#include <pcl/io/pcd_io.h>
#include <signal.h>

bool g_loopForever = true;

using namespace rtabmap;

class Reprocess_Server
{
protected:
    ros::NodeHandle n;

    actionlib::SimpleActionServer<cpp::RtabmapReprocessAction> as;
    std::string action_name;

    std::string inputDatabasePath;

    std::string outputDatabasePath;

    cpp::RtabmapReprocessResult result;

    int loopCount = 0;
    int proxCount = 0;
    int loopCountMotion = 0;
    int totalFrames = 0;
    int totalFramesMotion = 0;
    std::vector<float> previousLocalizationDistances;
    std::vector<float> odomDistances;
    std::vector<float> localizationVariations;
    std::vector<float> localizationAngleVariations;
    std::vector<float> localizationTime;
    std::map<int, Transform> odomTrajectoryPoses;
    std::multimap<int, Link> odomTrajectoryLinks;
    std::map<int, Transform> localizationPoses;
    bool exportPoses = false;
    int sessionCount = 0;

    int argc;
    char **argv;

public:
    Reprocess_Server(std::string name, int argc, char **argv) : as(n, name, boost::bind(&Reprocess_Server::reprocess, this, _1), false),
                                                                action_name(name), argc{argc}, argv{argv}
    {
        //register the goal and feeback callbacks

        as.start();
    }

    // void preemptCB()
    // {
    //     ROS_INFO("%s: Preempted", action_name.c_str());
    //     // set the action state to preempted
    //     as.setPreempted();
    // }

    // void goalCB()
    // {
    //     1_target_databasepath = as.acceptNewGoal()->databasepath;
    // }
    static void sighandler(int sig)
    {
        printf("\nSignal %d caught...\n", sig);
        g_loopForever = false;
    }

    void showLocalizationStats(const std::string &outputDatabasePath)
    {
        printf("Total localizations on previous session = %d/%d (Loop=%d, Prox=%d, In Motion=%d/%d)\n", loopCount + proxCount, totalFrames, loopCount, proxCount, loopCountMotion, totalFramesMotion);
        {
            float m = uMean(localizationTime);
            float var = uVariance(localizationTime, m);
            float stddev = -1;
            if (var > 0)
            {
                stddev = sqrt(var);
            }
            printf("Average localization time = %f ms (stddev=%f ms)\n", m, stddev);
        }
        if (localizationVariations.size() >= 2)
        {
            //ignore first localization
            localizationVariations = std::vector<float>(++localizationVariations.begin(), localizationVariations.end());
            localizationAngleVariations = std::vector<float>(++localizationAngleVariations.begin(), localizationAngleVariations.end());

            float m = uMean(localizationVariations);
            float max = uMax(localizationVariations);
            float var = uVariance(localizationVariations, m);
            float stddev = -1;
            if (var > 0)
            {
                stddev = sqrt(var);
            }
            float mA = uMean(localizationAngleVariations);
            float maxA = uMax(localizationAngleVariations);
            float varA = uVariance(localizationAngleVariations, mA);
            float stddevA = -1;
            if (varA > 0)
            {
                stddevA = sqrt(varA);
            }
            printf("Average localization variations = %f m, %f deg (stddev=%f m, %f deg) (max=%f m, %f deg)\n", m, mA, stddev, stddevA, max, maxA);
        }
        if (!previousLocalizationDistances.empty())
        {
            float m = uMean(previousLocalizationDistances);
            float var = uVariance(previousLocalizationDistances, m);
            float stddev = -1;
            if (var > 0)
            {
                stddev = sqrt(var);
            }
            printf("Average distance from previous localization = %f m (stddev=%f m)\n", m, stddev);
        }
        if (!odomDistances.empty())
        {
            float m = uMean(odomDistances);
            float var = uVariance(odomDistances, m);
            float stddev = -1;
            if (var > 0)
            {
                stddev = sqrt(var);
            }
            printf("Average odometry distances = %f m (stddev=%f m)\n", m, stddev);
        }

        if (exportPoses)
        {
            std::string outputPath = outputDatabasePath.substr(0, outputDatabasePath.size() - 3);
            std::string oName = outputPath + uFormat("_session_%d_odom.g2o", sessionCount);
            std::string lName = outputPath + uFormat("_session_%d_loc.g2o", sessionCount);
            graph::exportPoses(oName, 4, odomTrajectoryPoses, odomTrajectoryLinks);
            graph::exportPoses(lName, 4, localizationPoses, odomTrajectoryLinks);
            printf("Exported %s and %s\n", oName.c_str(), lName.c_str());
        }
        loopCount = 0;
        proxCount = 0;
        totalFrames = 0;
        loopCountMotion = 0;
        totalFramesMotion = 0;
        previousLocalizationDistances.clear();
        odomDistances.clear();
        localizationVariations.clear();
        localizationAngleVariations.clear();
        localizationTime.clear();
        odomTrajectoryPoses.clear();
        odomTrajectoryLinks.clear();
        localizationPoses.clear();
        ++sessionCount;
    }

    int reprocess(const cpp::RtabmapReprocessGoal::ConstPtr &goal)
    {
        signal(SIGABRT, &sighandler);
        signal(SIGTERM, &sighandler);
        signal(SIGINT, &sighandler);

        ULogger::setType(ULogger::kTypeConsole);
        ULogger::setLevel(ULogger::kError);

        ParametersMap customParameters = Parameters::parseArguments(argc, argv);
        bool assemble2dMap = false;
        bool assemble3dMap = false;
        bool assemble2dOctoMap = false;
        bool assemble3dOctoMap = false;
        bool useDatabaseRate = false;
        int startId = 0;
        int stopId = 0;
        int framesToSkip = 0;
        bool scanFromDepth = false;
        int scanDecimation = 1;
        float scanRangeMin = 0.0f;
        float scanRangeMax = 0.0f;
        float scanVoxelSize = 0;
        int scanNormalK = 0;
        float scanNormalRadius = 0.0f;
        ParametersMap configParameters;

        inputDatabasePath = "";
        for (auto val : goal->databasepaths)
            inputDatabasePath += val + ";";

        inputDatabasePath.erase(inputDatabasePath.size() - 1);

        outputDatabasePath = goal->goal_path;

        std::list<std::string> databases = uSplit(inputDatabasePath, ';');

        if (databases.empty())
        {
            printf("No input database \"%s\" detected!\n", inputDatabasePath.c_str());
            return 1;
        }

        for (std::list<std::string>::iterator iter = databases.begin(); iter != databases.end(); ++iter)
        {
            if (!UFile::exists(*iter))
            {
                printf("Input database \"%s\" doesn't exist!\n", iter->c_str());
                return 1;
            }

            if (UFile::getExtension(*iter).compare("db") != 0)
            {
                printf("File \"%s\" is not a database format (*.db)!\n", iter->c_str());
                return 1;
            }
        }

        if (UFile::getExtension(outputDatabasePath).compare("db") != 0)
        {
            printf("File \"%s\" is not a database format (*.db)!\n", outputDatabasePath.c_str());
            return 1;
        }

        if (UFile::exists(outputDatabasePath))
        {
            UFile::erase(outputDatabasePath);
        }

        // Get parameters of the first database
        DBDriver *dbDriver = DBDriver::create();
        if (!dbDriver->openConnection(databases.front(), false))
        {
            printf("Failed opening input database!\n");
            delete dbDriver;
            return 1;
        }

        ParametersMap parameters = dbDriver->getLastParameters();
        std::string targetVersion = dbDriver->getDatabaseVersion();

        parameters.insert(ParametersPair(Parameters::kDbTargetVersion(), targetVersion));
        if (parameters.empty())
        {
            printf("WARNING: Failed getting parameters from database, reprocessing will be done with default parameters! Database version may be too old (%s).\n", dbDriver->getDatabaseVersion().c_str());
        }
        if (customParameters.size())
        {
            printf("Custom parameters:\n");
            for (ParametersMap::iterator iter = customParameters.begin(); iter != customParameters.end(); ++iter)
            {
                printf("  %s\t= %s\n", iter->first.c_str(), iter->second.c_str());
            }
        }
        if ((configParameters.find(Parameters::kKpDetectorStrategy()) != configParameters.end() ||
             configParameters.find(Parameters::kVisFeatureType()) != configParameters.end() ||
             customParameters.find(Parameters::kKpDetectorStrategy()) != customParameters.end() ||
             customParameters.find(Parameters::kVisFeatureType()) != customParameters.end()) &&
            configParameters.find(Parameters::kMemUseOdomFeatures()) == configParameters.end() &&
            customParameters.find(Parameters::kMemUseOdomFeatures()) == customParameters.end())
        {
            bool useOdomFeatures = Parameters::defaultMemUseOdomFeatures();
            Parameters::parse(parameters, Parameters::kMemUseOdomFeatures(), useOdomFeatures);
            if (useOdomFeatures)
            {
                printf("[Warning] %s and/or %s are overwritten but parameter %s is true in the opened database. "
                       "Setting it to false for convenience to use the new selected feature detector. Set %s "
                       "explicitly to suppress this warning.\n",
                       Parameters::kKpDetectorStrategy().c_str(),
                       Parameters::kVisFeatureType().c_str(),
                       Parameters::kMemUseOdomFeatures().c_str(),
                       Parameters::kMemUseOdomFeatures().c_str());
                uInsert(parameters, ParametersPair(Parameters::kMemUseOdomFeatures(), "false"));
            }
        }
        uInsert(parameters, configParameters);
        uInsert(parameters, customParameters);

        bool incrementalMemory = Parameters::defaultMemIncrementalMemory();
        Parameters::parse(parameters, Parameters::kMemIncrementalMemory(), incrementalMemory);
        Parameters::parse(parameters, Parameters::kDbTargetVersion(), targetVersion);
        bool intermediateNodes = Parameters::defaultRtabmapCreateIntermediateNodes();
        Parameters::parse(parameters, Parameters::kRtabmapCreateIntermediateNodes(), intermediateNodes);

        int totalIds = 0;
        std::set<int> ids;
        dbDriver->getAllNodeIds(ids, false, false, !intermediateNodes);
        if (ids.empty())
        {
            printf("Input database doesn't have any nodes saved in it.\n");
            dbDriver->closeConnection(false);
            delete dbDriver;
            return 1;
        }
        if (!(!incrementalMemory && databases.size() > 1))
        {
            totalIds = ids.size();
        }
        dbDriver->closeConnection(false);

        // Count remaining ids in the other databases
        for (std::list<std::string>::iterator iter = ++databases.begin(); iter != databases.end(); ++iter)
        {
            if (!dbDriver->openConnection(*iter, false))
            {
                printf("Failed opening input database!\n");
                delete dbDriver;
                return 1;
            }
            ids.clear();
            dbDriver->getAllNodeIds(ids, false, false, !intermediateNodes);
            totalIds += ids.size();
            dbDriver->closeConnection(false);
        }
        delete dbDriver;
        dbDriver = 0;

        if (framesToSkip)
        {
            totalIds /= framesToSkip + 1;
        }

        std::string workingDirectory = UDirectory::getDir(outputDatabasePath);
        printf("Set working directory to \"%s\".\n", workingDirectory.c_str());
        if (!targetVersion.empty())
        {
            printf("Target database version: \"%s\" (set explicitly --%s \"\" to output with latest version.\n", targetVersion.c_str(), Parameters::kDbTargetVersion().c_str());
        }
        uInsert(parameters, ParametersPair(Parameters::kRtabmapWorkingDirectory(), workingDirectory));
        uInsert(parameters, ParametersPair(Parameters::kRtabmapPublishStats(), "true")); // to log status below

        if (!incrementalMemory && databases.size() > 1)
        {
            UFile::copy(databases.front(), outputDatabasePath);
            printf("Parameter \"%s\" is set to false, initializing RTAB-Map with \"%s\" for localization...\n", Parameters::kMemIncrementalMemory().c_str(), databases.front().c_str());
            databases.pop_front();
            inputDatabasePath = uJoin(databases, ";");
        }

        Rtabmap rtabmap;
        rtabmap.init(parameters, outputDatabasePath);

        bool rgbdEnabled = Parameters::defaultRGBDEnabled();
        Parameters::parse(parameters, Parameters::kRGBDEnabled(), rgbdEnabled);
        bool odometryIgnored = !rgbdEnabled;
        DBReader *dbReader = new DBReader(inputDatabasePath, useDatabaseRate ? -1 : 0, odometryIgnored, false, false, startId, -1, stopId, !intermediateNodes);
        dbReader->init();

        OccupancyGrid grid(parameters);
        grid.setCloudAssembling(assemble3dMap);

        float linearUpdate = Parameters::defaultRGBDLinearUpdate();
        float angularUpdate = Parameters::defaultRGBDAngularUpdate();
        Parameters::parse(parameters, Parameters::kRGBDLinearUpdate(), linearUpdate);
        Parameters::parse(parameters, Parameters::kRGBDAngularUpdate(), angularUpdate);

        printf("Reprocessing data of \"%s\"...\n", inputDatabasePath.c_str());
        std::map<std::string, float> globalMapStats;
        int processed = 0;
        CameraInfo info;
        SensorData data = dbReader->takeImage(&info);
        CameraThread camThread(dbReader, parameters); // take ownership of dbReader
        camThread.setScanParameters(scanFromDepth, scanDecimation, scanRangeMin, scanRangeMax, scanVoxelSize, scanNormalK, scanNormalRadius);

        //////////////////////////////////////////////////////////////////////////////////
        if (scanFromDepth)
        {
            data.setLaserScan(LaserScan());
        }
        camThread.postUpdate(&data, &info);
        Transform lastLocalizationOdomPose = info.odomPose;
        bool inMotion = true;
        ////////////////////////////////////////////////////////////////////////////////////
        //Go to 818
        while (data.isValid() && g_loopForever)
        {
            UTimer iterationTime;
            std::string status;
            if (!odometryIgnored && info.odomPose.isNull())
            {
                printf("Skipping node %d as it doesn't have odometry pose set.\n", data.id());
            }
            else
            {
                if (!odometryIgnored && !info.odomCovariance.empty() && info.odomCovariance.at<double>(0, 0) >= 9999)
                {
                    printf("High variance detected, triggering a new map...\n");
                    if (!incrementalMemory && processed > 0)
                    {
                        showLocalizationStats(outputDatabasePath);
                        lastLocalizationOdomPose = info.odomPose;
                    }
                    rtabmap.triggerNewMap();
                    inMotion = true;
                }
                UTimer t;
                if (!rtabmap.process(data, info.odomPose, info.odomCovariance, info.odomVelocity, globalMapStats))
                {
                    printf("Failed processing node %d.\n", data.id());
                    globalMapStats.clear();
                }
                else if (assemble2dMap || assemble3dMap || assemble2dOctoMap || assemble3dOctoMap)
                {
                    globalMapStats.clear();
                    double timeRtabmap = t.ticks();
                    double timeUpdateInit = 0.0;
                    double timeUpdateGrid = 0.0;
#ifdef RTABMAP_OCTOMAP
                    double timeUpdateOctoMap = 0.0;
#endif
                    const rtabmap::Statistics &stats = rtabmap.getStatistics();
                    if (stats.poses().size() && stats.getLastSignatureData().id())
                    {
                        int id = stats.poses().rbegin()->first;
                        if (id == stats.getLastSignatureData().id() &&
                            stats.getLastSignatureData().sensorData().gridCellSize() > 0.0f)
                        {
                            bool updateGridMap = false;
                            bool updateOctoMap = false;
                            if ((assemble2dMap || assemble3dMap) && grid.addedNodes().find(id) == grid.addedNodes().end())
                            {
                                updateGridMap = true;
                            }

                            if (updateGridMap || updateOctoMap)
                            {
                                cv::Mat ground, obstacles, empty;
                                stats.getLastSignatureData().sensorData().uncompressDataConst(0, 0, 0, 0, &ground, &obstacles, &empty);

                                timeUpdateInit = t.ticks();

                                if (updateGridMap)
                                {
                                    grid.addToCache(id, ground, obstacles, empty);
                                    grid.update(stats.poses());
                                    timeUpdateGrid = t.ticks() + timeUpdateInit;
                                }
                            }
                        }
                    }

                    globalMapStats.insert(std::make_pair(std::string("GlobalGrid/GridUpdate/ms"), timeUpdateGrid * 1000.0f));

                    globalMapStats.insert(std::make_pair(std::string("GlobalGrid/TotalWithRtabmap/ms"), (timeUpdateGrid + timeRtabmap) * 1000.0f));
                }
            }

            const rtabmap::Statistics &stats = rtabmap.getStatistics();
            int refId = stats.refImageId();
            int loopId = stats.loopClosureId() > 0 ? stats.loopClosureId() : stats.proximityDetectionId() > 0 ? stats.proximityDetectionId()
                                                                                                              : 0;
            int landmarkId = (int)uValue(stats.data(), rtabmap::Statistics::kLoopLandmark_detected(), 0.0f);
            int refMapId = stats.refImageMapId();
            ++totalFrames;

            if (inMotion)
            {
                ++totalFramesMotion;
            }
            if (loopId > 0)
            {
                if (stats.loopClosureId() > 0)
                {
                    ++loopCount;
                }
                else
                {
                    ++proxCount;
                }
                if (inMotion)
                {
                    ++loopCountMotion;
                }
                int loopMapId = stats.loopClosureId() > 0 ? stats.loopClosureMapId() : stats.proximityDetectionMapId();
                printf("Processed %d/%d nodes [id=%d map=%d]... %dms %s on %d [%d]\n", ++processed, totalIds, refId, refMapId, int(iterationTime.ticks() * 1000), stats.loopClosureId() > 0 ? "Loop" : "Prox", loopId, loopMapId);
            }
            else if (landmarkId != 0)
            {
                printf("Processed %d/%d nodes [id=%d map=%d]... %dms Loop on landmark %d\n", ++processed, totalIds, refId, refMapId, int(iterationTime.ticks() * 1000), landmarkId);
            }
            else
            {
                printf("Processed %d/%d nodes [id=%d map=%d]... %dms\n", ++processed, totalIds, refId, refMapId, int(iterationTime.ticks() * 1000));
            }

            // Here we accumulate statistics about distance from last localization
            if (!incrementalMemory &&
                !lastLocalizationOdomPose.isNull() &&
                !info.odomPose.isNull())
            {
                if (loopId > 0 || landmarkId != 0)
                {
                    previousLocalizationDistances.push_back(lastLocalizationOdomPose.getDistance(info.odomPose));
                    lastLocalizationOdomPose = info.odomPose;
                }
            }
            if (!incrementalMemory)
            {
                float totalTime = uValue(stats.data(), rtabmap::Statistics::kTimingTotal(), 0.0f);
                localizationTime.push_back(totalTime);
                if (stats.data().find(Statistics::kLoopOdom_correction_norm()) != stats.data().end())
                {
                    localizationVariations.push_back(stats.data().at(Statistics::kLoopOdom_correction_norm()));
                    localizationAngleVariations.push_back(stats.data().at(Statistics::kLoopOdom_correction_angle()));
                }

                if (exportPoses && !info.odomPose.isNull())
                {
                    if (!odomTrajectoryPoses.empty())
                    {
                        int previousId = odomTrajectoryPoses.rbegin()->first;
                        odomTrajectoryLinks.insert(std::make_pair(previousId, Link(previousId, refId, Link::kNeighbor, odomTrajectoryPoses.rbegin()->second.inverse() * info.odomPose, info.odomCovariance)));
                    }
                    odomTrajectoryPoses.insert(std::make_pair(refId, info.odomPose));
                    localizationPoses.insert(std::make_pair(refId, stats.mapCorrection() * info.odomPose));
                }
            }

            Transform odomPose = info.odomPose;

            if (framesToSkip > 0)
            {
                int skippedFrames = framesToSkip;
                while (skippedFrames-- > 0)
                {
                    data = dbReader->takeImage(&info);
                    if (!odometryIgnored && !info.odomCovariance.empty() && info.odomCovariance.at<double>(0, 0) >= 9999)
                    {
                        printf("High variance detected, triggering a new map...\n");
                        if (!incrementalMemory && processed > 0)
                        {
                            showLocalizationStats(outputDatabasePath);
                            lastLocalizationOdomPose = info.odomPose;
                        }
                        rtabmap.triggerNewMap();
                    }
                }
            }

            data = dbReader->takeImage(&info);
            if (scanFromDepth)
            {
                data.setLaserScan(LaserScan());
            }
            camThread.postUpdate(&data, &info);

            inMotion = true;
            if (!incrementalMemory &&
                !odomPose.isNull() &&
                !info.odomPose.isNull())
            {
                float distance = odomPose.getDistance(info.odomPose);
                float angle = (odomPose.inverse() * info.odomPose).getAngle();
                odomDistances.push_back(distance);
                if (distance < linearUpdate && angle <= angularUpdate)
                {
                    inMotion = false;
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        int databasesMerged = 0;
        if (!incrementalMemory)
        {
            showLocalizationStats(outputDatabasePath);
        }
        else
        {
            printf("Total loop closures = %d (Loop=%d, Prox=%d, In Motion=%d/%d)\n", loopCount + proxCount, loopCount, proxCount, loopCountMotion, totalFramesMotion);

            if (databases.size() > 1)
            {
                std::map<int, Transform> poses;
                std::multimap<int, Link> constraints;
                rtabmap.getGraph(poses, constraints, 0, 1, 0, false, false, false, false, false, false);
                std::set<int> mapIds;
                for (std::map<int, Transform>::iterator iter = poses.begin(); iter != poses.end(); ++iter)
                {
                    int id;
                    if ((id = rtabmap.getMemory()->getMapId(iter->first, true)) >= 0)
                    {
                        mapIds.insert(id);
                    }
                }
                databasesMerged = mapIds.size();
            }
        }

        printf("Closing database \"%s\"...\n", outputDatabasePath.c_str());
        rtabmap.close(true);
        printf("Closing database \"%s\"... done!\n", outputDatabasePath.c_str());
        return 1;
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "reprocess_server_node");

    Reprocess_Server server_agent("RtabmapReprocess_node", argc, argv);

    ros::spin();

    return 0;
}

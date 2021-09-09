#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/core/CameraRGB.h"
#include <opencv2/core/core.hpp>
#include "rtabmap/utilite/UFile.h"
#include <stdio.h>
#include <iostream>

void showUsage()
{
   printf("\nUsage:\n"
   		"rtabmap-bow_mapping [options] \"path\"\n"
   		"  path       Path to a directory of images\n "
   		"  Options:"
   		"     -l   localization mode: use already built RTAB-Map database to localize\n ");
   exit(1);
}

int main(int argc, char * argv[])
{
   //ULogger::setType(ULogger::kTypeConsole);
   //ULogger::setLevel(ULogger::kDebug);

   std::string path;
   bool localizationMode = false;

   if(argc < 2)
   {
   	showUsage();
   }

   for(int i=1; i<argc-1; ++i)
   {
   	if(strcmp(argv[i], "-l") == 0)
   	{
   		localizationMode = true;
   	}
   	else
   	{
   		printf("Unrecognized option \"%s\"\n", argv[i]);
   		showUsage();
   	}
   }
   path = argv[argc-1];

   // rtabmap::Camera is simply a convenience wrapper of OpenCV cv::VideoCapture and cv::imread
   rtabmap::CameraImages camera(path);
   if(!camera.init())
   {
   	printf("Camera init failed, using path \"%s\"\n", path.c_str());
   	exit(1);
   }

   // Create RTAB-Map
   rtabmap::Rtabmap rtabmap;

   // Set the time threshold
   rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

   // To set other parameters, the Parameters interface must be used (Parameters.h).
   // Example here to change the loop closure threshold (default 0.15).
   // Lower the threshold, more loop closures are detected but there is more chance of false positives.
   rtabmap::ParametersMap parameters;
   parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.11"));

   // The time threshold set above is also a parameter, one could have set it the same way:
   //   parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapTimeThr(), "700"));
   // Or SURF hessian treshold:
   //   parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kSURFHessianThreshold(), "150"));

   // Appearance-based only, disable RGB-D mode
   parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

   std::string databasePath = "/home/ayumi/Documents/RTAB-Map/"+rtabmap::Parameters::getDefaultDatabaseName();
   if(localizationMode)
   {
   	parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kMemIncrementalMemory(), "false"));
   	parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kKpIncrementalDictionary(), "false"));
   	parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kMemSTMSize(), "1"));
   }
   else
   {
   	// delete previous database if there's one...
   	UFile::erase(databasePath);
   }

   // Initialize rtabmap: delete/create database...
   rtabmap.init(parameters, databasePath);

   // Process each image of the directory...
   printf("\nProcessing images... from directory \"%s\"\n", path.c_str());

   int countLoopDetected=0;
   int i=0;
   rtabmap::SensorData data = camera.takeImage();
   int nextIndex = rtabmap.getLastLocationId()+1;
   while(!data.imageRaw().empty())
   {
   	// Process image : Main loop of RTAB-Map
   	rtabmap.process(data.imageRaw(), nextIndex);

   	// Check if a loop closure is detected and print some info
   	if(rtabmap.getLoopClosureId())
   	{
   		++countLoopDetected;
   	}
   	++i;
   	if(rtabmap.getLoopClosureId())
   	{
   		printf(" #%d ptime(%fs) STM(%d) WM(%d) hyp(%d) value(%.2f) *LOOP %d->%d*\n",
   				i,
   				rtabmap.getLastProcessTime(),
   				(int)rtabmap.getSTM().size(), // short-term memory
   				(int)rtabmap.getWM().size(), // working memory
   				rtabmap.getLoopClosureId(),
   				rtabmap.getLoopClosureValue(),
   				nextIndex,
   				rtabmap.getLoopClosureId());
   	}
   	else
   	{
   		printf(" #%d ptime(%fs) STM(%d) WM(%d) hyp(%d) value(%.2f)\n",
   				i,
   				rtabmap.getLastProcessTime(),
   				(int)rtabmap.getSTM().size(), // short-term memory
   				(int)rtabmap.getWM().size(), // working memory
   				rtabmap.getHighestHypothesisId(), // highest loop closure hypothesis
   				rtabmap.getLoopClosureValue());
   	}

   	++nextIndex;

   	//Get next image
   	data = camera.takeImage();
   }

   printf("Processing images completed. Loop closures found = %d\n", countLoopDetected);

   // Generate a graph for visualization with Graphiz
   rtabmap.generateDOTGraph("Graph.dot");
   printf("Generated graph \"Graph.dot\", viewable with Graphiz using \"neato -Tpdf Graph.dot -o out.pdf\"\n");

   // Cleanup... save database and logs
   printf("Saving Long-Term Memory to \"rtabmap.db\"...\n");
   rtabmap.close();
   printf("----------------------------------------------------");
   printf("----------------------------------------------------");
   printf("----------------------------------------------------");
   printf("Again!!");
   // Initialize rtabmap: delete/create database...
   databasePath="/home/ayumi/Documents/RTAB-Map/rtabmap1.db";
   rtabmap.init(parameters, databasePath);
   path="test3";
   //👇This!!!
   camera=path;
   camera.init();
   data=camera.takeImage();
   nextIndex=1;
   // Process each image of the directory...
   printf("\nProcessing images... from directory \"%s\"\n", path.c_str());

   countLoopDetected=0;
   i=0;
   std::cout<<data.imageRaw().empty()<<std::endl;
   while(!data.imageRaw().empty())
   {
   	// Process image : Main loop of RTAB-Map
   	rtabmap.process(data.imageRaw(), nextIndex);

   	// Check if a loop closure is detected and print some info
   	if(rtabmap.getLoopClosureId())
   	{
   		++countLoopDetected;
   	}
   	++i;
   	if(rtabmap.getLoopClosureId())
   	{
   		printf(" #%d ptime(%fs) STM(%d) WM(%d) hyp(%d) value(%.2f) *LOOP %d->%d*\n",
   				i,
   				rtabmap.getLastProcessTime(),
   				(int)rtabmap.getSTM().size(), // short-term memory
   				(int)rtabmap.getWM().size(), // working memory
   				rtabmap.getLoopClosureId(),
   				rtabmap.getLoopClosureValue(),
   				nextIndex,
   				rtabmap.getLoopClosureId());
   	}
   	else
   	{
   		printf(" #%d ptime(%fs) STM(%d) WM(%d) hyp(%d) value(%.2f)\n",
   				i,
   				rtabmap.getLastProcessTime(),
   				(int)rtabmap.getSTM().size(), // short-term memory
   				(int)rtabmap.getWM().size(), // working memory
   				rtabmap.getHighestHypothesisId(), // highest loop closure hypothesis
   				rtabmap.getLoopClosureValue());
   	}

   	++nextIndex;

   	//Get next image
   	data = camera.takeImage();
   }
   printf("Processing images completed. Loop closures found = %d\n", countLoopDetected);
   rtabmap.close();

   return 0;
}
//512×384

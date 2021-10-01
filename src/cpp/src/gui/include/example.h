#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QApplication>
#include <QtCore/QDir>
#include <QTimer>
#include <QObject>
#include <iostream>
#include <vtk-7.1/vtkObject.h>
#include <string>
#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include "rtabmap/utilite/UEventsManager.h"
#include "rtabmap/core/RtabmapThread.h"
#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/utilite/UObjDeletionThread.h"
#include "rtabmap/utilite/UFile.h"
#include "rtabmap/utilite/UConversion.h"
#include "rtabmap/gui/MainWindow.h"


class overloaded_window :public rtabmap::MainWindow
{
    Q_OBJECT
    
public:
    overloaded_window(int argc,char **argv);
    rtabmap::ParametersMap parameters;
    void gui_Callback(const std_msgs::Int32::ConstPtr &msg);
    // void dir_callback(const std_msgs::String::ConstPtr &msg);

public slots:
    void open_database(const rtabmap::ParametersMap &overridedParameters);
    void close_database();
    void status_checker();

signals:
    void valueChanged(int);
    void signal0(void);
    

private:
    QTimer *mytimer;
    QTimer *sub_timer;
    ros::NodeHandle n;
    ros::Subscriber sub;

    int status{0};
    int previous_status=0;
    QString database_path{"/home/ayumi/.ros/rtabmap.db"};


};

#endif

#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QApplication>
#include <QtCore/QDir>
#include <QTimer>
#include <QObject>
#include <iostream>
#include <vtk-7.1/vtkObject.h>
#include <string>
#include "listener.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rtabmap/utilite/UEventsManager.h"
#include "rtabmap/core/RtabmapThread.h"
#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/utilite/UObjDeletionThread.h"
#include "rtabmap/utilite/UFile.h"
#include "rtabmap/utilite/UConversion.h"
#include "rtabmap/gui/MainWindow.h"


class overloaded_window : public listener_node, public rtabmap::MainWindow
{
    Q_OBJECT
public:
    overloaded_window(int argc,char **argv);
    rtabmap::ParametersMap parameters;

public slots:
    void open_database(const rtabmap::ParametersMap &overridedParameters);
    void close_database();
    void status_checker();

signals:
    void valueChanged(int){}
    void signal0(){}
    

private:
    QTimer *mytimer;
    QTimer *sub_timer;


};

#endif

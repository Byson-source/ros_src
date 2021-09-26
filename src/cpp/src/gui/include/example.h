#ifndef EXAMPLE_H
#define EXAMPLE_H
#include "object.h"
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

class overloaded_window : public rtabmap::MainWindow,public listener_node
{
    Q_OBJECT
public:
    overloaded_window(int,char);

public slots:
    void open_database();
    void close_database();
    void status_checker();

signals:
    void valueChanged(int);
    void signal0();
    

private:
    QTimer *mytimer;
    QTimer *sub_timer;

};

#endif

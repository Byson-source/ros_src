#include "example.h"
#include <string>
#include <iostream>
#include <QTimer>
#include <QApplication>
#include <QtCore/QDir>
#include "rtabmap/utilite/UEventsManager.h"
#include "rtabmap/core/RtabmapThread.h"
#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/utilite/UObjDeletionThread.h"
#include "rtabmap/utilite/UFile.h"
#include "rtabmap/utilite/UConversion.h"
#include "rtabmap/gui/MainWindow.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#define TIME_TO_CONNECT_TO_DATABASE 1000
#define TIME_TO_EMIT 1
#define GUI_SWITCH_TOPIC "gui_info"
#define STORED_DIR_TOPIC "stored_dir"

overloaded_window::overloaded_window(int argc, char **argv)
{
    sub = n.subscribe(GUI_SWITCH_TOPIC, 10, &overloaded_window::gui_Callback, this);

    parameters = rtabmap::Parameters::parseArguments(argc, argv, false);

    mytimer = new QTimer(this);
    mytimer->setSingleShot(true);
    mytimer->setInterval(TIME_TO_CONNECT_TO_DATABASE);
    mytimer->setTimerType(Qt::PreciseTimer);

    sub_timer = new QTimer(this);
    sub_timer->setSingleShot(false);
    sub_timer->setInterval(TIME_TO_EMIT);
    sub_timer->setTimerType(Qt::PreciseTimer);
    sub_timer->start();

    QObject::connect(sub_timer, SIGNAL(timeout()), this, SLOT(overloaded_window::status_checker()));
    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(open_database()));
    QObject::connect(this, SIGNAL(signal0()), mytimer, SLOT(start()));
    QObject::connect(mytimer, SIGNAL(timeout()), SLOT(close_database()));
}

void overloaded_window::gui_Callback(const std_msgs::Int32::ConstPtr &msg)
{
    // previous_status=status;
    if (msg->data == 1)
        status = 1;
    else
        status = 0;
}


void overloaded_window::open_database(const rtabmap::ParametersMap &overridedParameters)
{
    openDatabase(database_path, overridedParameters);
    emit signal0();
}

void overloaded_window::close_database()
{
    closeDatabase();
    status = 0;
    previous_status = 0;
}


void overloaded_window::status_checker()
{
    // if (!previous_status == status && status == 1)
    if (status == 1 && !previous_status == status)
    {
        std::cout << "GUI is launching..." << std::endl;
        emit valueChanged(1);

        previous_status = 1;
    }
    else
        return;
}

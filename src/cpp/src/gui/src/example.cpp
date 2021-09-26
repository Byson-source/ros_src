#include "example.h"
#include <string>
#include "object.h"
#include <iostream>
#include <QTimer>
#include <QApplication>
#include <QtCore/QDir>
#define TIME_TO_CONNECT_TO_DATABASE 1000
#define TIME_TO_EMIT 1

overloaded_window::overloaded_window(int &argc, char **argv)
{
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
    QObject::connect(mytimer, SIGNAL(timeout()), close_database());


}

overloaded_window::open_database()
{
    openDatabase(database_path);
    emit signal0();
}

overloaded_window::close_database()
{
    closeDatabase();
    status=0;
    previous_status=0;
}

overloaded_window::status_checker()
{
    // if (!previous_status == status && status == 1)
    if (status == 1 && !previous_status == status)
    {
        std::cout << "GUI is launching..." << std::endl;
        emit valueChanged(1);

        previous_status=1;
    }
    else
        return;
}

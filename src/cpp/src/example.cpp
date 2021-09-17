#include "example.h"
#include <string>
#include "object.h"
#include <iostream>
#include <QTimer>
#define TIME_TO_SHOW_YOU 1000
#define TIME_TO_EMIT 1

overloaded_window::overloaded_window()
{
    mytimer=new QTimer(this);
    mytimer->setSingleShot(true);
    mytimer->setInterval(TIME_TO_SHOW_YOU);
    mytimer->setTimerType(Qt::PreciseTimer);
    connect(mytimer,SIGNAL(timeout()),this,SLOT(close_database()));

    sub_timer=new QTimer(this);
    sub_timer->setSingleShot(false);
    sub_timer->setInterval(TIME_TO_EMIT);
    sub_timer->setTimerType(Qt::PreciseTimer);
    connect(sub_timer,SIGNAL(timeout()),this,SLOT());
    
    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(open_database()));
    QObject::connect(this, SIGNAL(valueChanged(int)), mytimer, SLOT(start()));
    QObject::connect(mytimer,SIGNAL(timeout()),close_database());
}

overloaded_window::open_database()
{
    openDatabase(database_path);
}
overloaded_window::status_checker()
{
    if (!previous_status == status && status == 1)
    {
        std::cout<<"GUI is launching..."<<std::endl;
        emit valueChanged(1);
    
    }
    else
        return;

}

overloaded_window::close_database(){
    closeDatabase();
}
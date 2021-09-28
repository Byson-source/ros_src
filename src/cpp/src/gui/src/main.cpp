#include <QApplication>
#include <QtCore/QDir>
#include "rtabmap/utilite/UEventsManager.h"
#include "rtabmap/core/RtabmapThread.h"
#include "rtabmap/core/Rtabmap.h"
#include "example.h"
#include <QMessageBox>
#include "rtabmap/utilite/UObjDeletionThread.h"
#include "rtabmap/utilite/UFile.h"
#include "rtabmap/utilite/UConversion.h"
#include <QTimer>
#include<iostream>
#include"example.h"

#include <vtk-7.1/vtkObject.h>

#ifdef RTABMAP_PYTHON
#include "rtabmap/core/PythonInterface.h"
#endif

using namespace rtabmap;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "GUI_node");
    /* Set logger type */
    ULogger::setType(ULogger::kTypeConsole);
    ULogger::setLevel(ULogger::kWarning);


    /* Create tasks */
    QApplication * app = new QApplication(argc, argv);
    app->setStyleSheet("QMessageBox { messagebox-text-interaction-flags: 5; }"); // selectable message box

    overloaded_window * mainWindow = new overloaded_window(argc,argv);
    app->installEventFilter(mainWindow); // to catch FileOpen events.

    std::string database;
    for(int i=1; i<argc; ++i)
    {
        std::string value = uReplaceChar(argv[i], '~', UDirectory::homeDir());
        if(UFile::exists(value) &&
           UFile::getExtension(value).compare("db") == 0)
        {
            database = value;
        }
    }

    printf("Program started...\n");

    UEventsManager::addHandler(mainWindow);

    /* Start thread's task */
    if(mainWindow->isSavedMaximized())
    {
        mainWindow->showMaximized();
    }
    else
    {
        mainWindow->show();
    }

    RtabmapThread * rtabmap = new RtabmapThread(new Rtabmap());
    rtabmap->start(); // start it not initialized... will be initialized by event from the gui
    UEventsManager::addHandler(rtabmap);

    if(!database.empty())
    {
        mainWindow->openDatabase(database.c_str(), mainWindow->parameters);
    }
    else if(mainWindow->parameters.size())
    {
        mainWindow->updateParameters(mainWindow->parameters);
    }

    // Now wait for application to finish
    app->connect( app, SIGNAL( lastWindowClosed() ),
                app, SLOT( quit() ) );
    app->exec();// MUST be called by the Main Thread

    /* Remove handlers */
    UEventsManager::removeHandler(mainWindow);
    UEventsManager::removeHandler(rtabmap);

    rtabmap->join(true);

    printf("Closing RTAB-Map...\n");
    delete rtabmap;
    delete mainWindow;
    delete app;
    printf("All done!\n");

    return 0;
}

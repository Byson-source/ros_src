#include <QApplication>
#include <QtCore/QDir>
#include "rtabmap/utilite/UEventsManager.h"
#include "rtabmap/core/RtabmapThread.h"
#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/gui/MainWindow.h"
#include <QMessageBox>
#include "rtabmap/utilite/UObjDeletionThread.h"
#include "rtabmap/utilite/UFile.h"
#include "rtabmap/utilite/UConversion.h"

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <vtk-7.1/vtkObject.h>

#ifdef RTABMAP_PYTHON
#include "rtabmap/core/PythonInterface.h"
#endif

using namespace rtabmap;

void guicallback(const std_msgs::String::ConstPtr &msg)
{
	if
}

int main(int argc, char *argv[])
{
	/* Set logger type */
	ULogger::setType(ULogger::kTypeConsole);
	ULogger::setLevel(ULogger::kWarning);

#ifdef WIN32
	CoInitialize(nullptr);
#endif

#ifdef RTABMAP_PYTHON
	PythonInterface python; // Make sure we initialize python in main thread
#endif

#if VTK_MAJOR_VERSION >= 8
	vtkObject::GlobalWarningDisplayOff();
#endif

	/* Create tasks */
	QApplication *app = new QApplication(argc, argv);
	QTimer::singleShot(10000, app, SLOT(quit()));
	app->setStyleSheet("QMessageBox { messagebox-text-interaction-flags: 5; }"); // selectable message box

	ParametersMap parameters = Parameters::parseArguments(argc, argv, false);
	MainWindow *mainWindow = new MainWindow();
	app->installEventFilter(mainWindow); // to catch FileOpen events.

	std::string database;
	for (int i = 1; i < argc; ++i)
	{
		std::string value = uReplaceChar(argv[i], '~', UDirectory::homeDir());
		if (UFile::exists(value) &&
			UFile::getExtension(value).compare("db") == 0)
		{
			database = value;
		}
	}

	printf("Program started...\n");

	UEventsManager::addHandler(mainWindow);

	/* Start thread's task */
	if (mainWindow->isSavedMaximized())
	{
		mainWindow->showMaximized();
	}
	else
	{
		mainWindow->show();
	}

	RtabmapThread *rtabmap = new RtabmapThread(new Rtabmap());
	rtabmap->start(); // start it not initialized... will be initialized by event from the gui
	UEventsManager::addHandler(rtabmap);

	for (int i = 1; i < argc; ++i)
	{
		std::string value = uReplaceChar(argv[i], '~', UDirectory::homeDir());
		if (UFile::exists(value) &&
			UFile::getExtension(value).compare("db") == 0)
		{
			database = value;
		}
	}

	if (!database.empty())
	{
		mainWindow->openDatabase(database.c_str(), parameters);
	}
	else if (parameters.size())
	{
		mainWindow->updateParameters(parameters);
	}

	// Now wait for application to finish
	app->connect(app, SIGNAL(lastWindowClosed()),
				 app, SLOT(quit()));
	app->exec(); // MUST be called by the Main Thread

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

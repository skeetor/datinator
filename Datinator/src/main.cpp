/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <iostream>
#include <string>

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <QDir>
#include <QTextStream>
#include <QtCore/QSettings>
#include <QtCore/QList>
#include <QtCore/QStandardPaths>

#include <plugins/idata_container_reader.h>
#include <plugins/idata_container_writer.h>

/*#include "support/unicode/unicode_types.h"
#include "support/helper/hexdump.h"
bool hexdumpToFile(supportlib::string_t const &oInput, supportlib::string_t const &oOutput);*/

#include "application.h"
#include "gui/main_frame/main_window_gui.moc"

#define DEFAULT_CONFIG_FILE		"datinator.ini"

static const int MAX_CONSOLE_LINES = 500;

static void RedirectIOToConsole(void);
static void parseCommandline(int argc, char *argv[]);

Application *Application::mApplication = NULL;
bool gConsoleMode = false;
void updatePath(QString oPath, QString oExe);

void replaceAll(std::string &str, const std::string &from, const std::string &to)
{
	if(from.empty())
		return;

	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

int main(int argc, char *argv[])
{
    Application *app = Application::createInstance(argc, argv);
    parseCommandline(argc, argv);

#ifdef _DEBUG
	gConsoleMode = true;
#endif // DEBUG

	if(gConsoleMode)
	{
#ifdef _WINDOWS
		if(GetConsoleWindow() == NULL)
			AllocConsole();

		RedirectIOToConsole();
		std::cout << "Datinator console activated" << std::endl;
#endif // _WINDOWS
	}

    updatePath(getenv("PATH"), QApplication::applicationDirPath());

	QSettings &settings = app->getPropertyFile();
	MainWindow frame;

	std::cout << "Datinator started from " << supportlib::string::QtStringToStringT(QDir::currentPath()) << "\n" << std::endl;

	frame.show();
	frame.reloadPlugins();
	frame.restore(settings);

	int ret = app->exec();
	frame.store(settings);
	settings.sync();

//	delete app;

	return ret;
}

Application *Application::createInstance(int argc, char *argv[])
{
	if(mApplication == NULL)
		mApplication = new Application(argc, argv);

	return mApplication;
}

Application &Application::getInstance(void)
{
	return *Application::createInstance(0, NULL);
}

Application::Application(int argc, char *argv[])
: QApplication(argc, argv)
{
	mPropertyFile = NULL;
}

Application::~Application(void)
{
	if(mPropertyFile != NULL)
		delete mPropertyFile;
}

QSettings &Application::getPropertyFile(void)
{
	if(mPropertyFile == NULL)
    {
		QString p;
		QList<QString> locs = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
		p = locs.at(0) + "/";

#ifdef _LINUX
		p += ".";
#endif // _LINUX

		p += DEFAULT_CONFIG_FILE;

#ifdef _DEBUG
		p += "_debug";
#endif // _DEBUG

		std::cout << "Propertyfile: " << supportlib::string::QtStringToStringT(p) << "\n" << std::endl;
		mPropertyFile = new QSettings(p, QSettings::IniFormat);
    }

	return *mPropertyFile;
}

static void parseCommandline(int argc, char *argv[])
{
	for(int i = 1; i < argc; i++)
	{
		QString arg = argv[i];
		if(arg == "-console")
			gConsoleMode = true;
	}
}

#ifdef _WINDOWS
static void RedirectIOToConsole(void)
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
	coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}

void updatePath(QString oPath, QString oExe)
{
    QString path = oPath;
	path.replace("\\", "/").toUpper();

	if(!path.contains(oExe.toUpper()+";"))
	{
		oExe = oExe.replace("/", "\\");
		oPath = oExe+";"+oPath;
		std::string env_path = "PATH="+oPath.toStdString();
		std::cout << "Path updated.\n" << env_path << std::endl;
		putenv(env_path.c_str());
	}
}
#endif // _WINDOWS

#ifdef _LINUX
void updatePath(QString oPath, QString oExe)
{
	if(!oPath.contains(oExe.toUpper()+":"))
	{
		oPath = oExe+":"+oPath;
		std::string env_path = oPath.toStdString();
		std::cout << "Path updated.\n" << env_path << std::endl;
		setenv("PATH", env_path.c_str(), 1);
	}
}
#endif // _LINUX

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <QtWidgets/qapplication.h>

class Application : public QApplication
{
public:
	Application(int argc, char *argv[]);
	virtual ~Application(void);

	static Application &getInstance(void);
	static Application *createInstance(int argc, char *argv[]);

	QSettings &getPropertyFile(void);

private:
	static Application *mApplication;
	QSettings *mPropertyFile;
};

#endif // APPLICATION_H_INCLUDED

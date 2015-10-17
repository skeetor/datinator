/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef _PROGRESS_H_INCLUDED
#define _PROGRESS_H_INCLUDED

#include "datinator_types.h"
#include "plugin/plugin_dll_api.h"
#include <support/logging/logging.h>

#define ErrorMessage(t, u, m) Progress::addMessage((t), (u), (m), 0, __FILE__, __LINE__, __func__)

class QProgressDialog;

class PLUGIN_DLL_EXPORT Progress
{
public:
	Progress(StdString const &oText, StdString const &oTitle = "");
	virtual ~Progress(void);
	QProgressDialog *operator->(void);

	void showProgress(StdString const &oText, StdString const &oTitle = "");
	void closeProgress(void);

	static void PLUGIN_DLL_EXPORT addMessage(spt::logging::LoggingItem::LogType nType, StdString const &oUnit, StdString const &oMessage, unsigned int nTimestamp, StdString const &oFile, int nLine, StdString const &oMethod);
	void showMessages(void);
	void clearMessages(void);
	bool hasMessages(void);

	static void PLUGIN_DLL_EXPORT setParent(QWidget *oMainWindow);

protected:
	QWidget *getMainWindow(void);
};

#endif // _PROGRESS_H_INCLUDED

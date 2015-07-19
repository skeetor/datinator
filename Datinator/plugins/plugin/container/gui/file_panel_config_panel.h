/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef FILE_PANEL_CONFIG_PANEL_H_INCLUDED
#define FILE_PANEL_CONFIG_PANEL_H_INCLUDED

#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>

#include "plugin/plugin_dll_api.h"

class PLUGIN_DLL_EXPORT FilePanelConfigPanel
	: public QFrame
{
public:
	FilePanelConfigPanel(QWidget *oParentPanel = NULL);
	virtual ~FilePanelConfigPanel(void);
};

#endif // FILE_PANEL_CONFIG_PANEL_H_INCLUDED

/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef QT_GUI_HELPER_H
#define QT_GUI_HELPER_H

#include <QtCore/QRect>

#include "support_qt/support_qt_dll_api.h"

class QString;
class QPushButton;
class QLineEdit;
class QWidget;

namespace supportlib
{
	namespace image
	{
		extern const unsigned char *button_add;
		extern int button_add_length;
		extern const unsigned char *button_delete;
		extern int button_delete_length;
		extern const unsigned char *button_up;
		extern int button_up_length;
		extern unsigned char *button_down;
		extern int button_down_length;
	}

	namespace gui
	{
		SUPPORT_QT_DLL_EXPORT void setButtonIcon(unsigned char const *oImageData, int nBufferLen, QPushButton *oButton, QString const &oToolTip);
		SUPPORT_QT_DLL_EXPORT void setButtonIcon(QString const &oImageData, QPushButton *oButton, QString const &oToolTip);

		SUPPORT_QT_DLL_EXPORT void setEditable(QLineEdit *oEdit, bool bEditable);
		SUPPORT_QT_DLL_EXPORT void center(QWidget *oParent, QWidget *oChild, bool bKeepWidth = false, bool bKeepHeight = false);
	}
}

#endif // QT_GUI_HELPER_H

/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

#include <QtGui/QPixmap>
#include <QtGui/QImageReader>
#include <QtGui/QColor>

#include <QtCore/QByteArray>
#include <QtCore/QBuffer>

#include "support_qt/helper/gui_helper.h"
#include "support/helper/string.h"

void spt::gui::setEditable(QLineEdit *oEdit, bool bEditable)
{
	QPalette p;
	oEdit->setReadOnly(!bEditable);
	if(bEditable)
		oEdit->setPalette(p);
	else
	{
		QColor col = p.color(QPalette::Button);
		p.setColor(QPalette::Base, col);
		p.setColor(QPalette::Text, Qt::black);
		oEdit->setPalette(p);
	}
}

void spt::gui::setButtonIcon(spt::string::string_t const &oIconPath, QPushButton *oButton, spt::string::string_t const &oToolTip)
{
	QPixmap pixmap(spt::string::toQt(oIconPath));
	QIcon ButtonIcon(pixmap);
	oButton->setIcon(ButtonIcon);
	oButton->setIconSize(pixmap.rect().size());
	oButton->setToolTip(spt::string::toQt(oToolTip));
}

void spt::gui::setButtonIcon(unsigned char const *oImageData, int nBufferLen, QPushButton *oButton, spt::string::string_t const &oToolTip)
{
 	QImage img;
 	img.loadFromData(oImageData, nBufferLen);
 	QPixmap pixmap = QPixmap::fromImage(img);
	QIcon icon(pixmap);
	oButton->setIcon(icon);
	oButton->setIconSize(pixmap.rect().size());
	oButton->setToolTip(spt::string::toQt(oToolTip));
}

void spt::gui::center(QWidget *oParent, QWidget *oChild, bool bKeepWidth, bool bKeepHeight)
{
	int pw = oParent->width();
	int ph = oParent->height();
	int mw = oChild->width();
	int mh = oChild->height();
	int cw = mw;
	int ch = mh;

	if(!bKeepWidth)
		cw = (pw - mw)/2;

	if(!bKeepHeight)
		ch = (ph - mh)/2;

	QPoint pos = oParent->pos();
	int cx = ((pw - cw)/2)+pos.x();
	int cy = ((ph - ch)/2)+pos.y();

	oChild->move(cx, cy);
	oChild->resize(cw, ch);
}

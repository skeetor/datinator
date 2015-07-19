/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * RestorableWindow (c) by Gerhard W. Gruber in 2014
 *
 * A helper class to save and restore windows to it's current size
 * and position. It also includes support for specific controls.
 *
 *******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QDesktopWidget>

#include "support_qt/window/restorable_window.h"

Q_DECLARE_METATYPE(QList<int>)

RestorableWindow::RestorableWindow(void)
{
}

RestorableWindow::~RestorableWindow(void)
{
}

void RestorableWindow::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	for(QPair<QString, QWidget *> &p : mWidgets)
		storeWidget(oPropertyFile, oPrefix+p.first, p.second);

	for(QPair<QString, QSplitter *> &p : mSplitters)
		storeSplitter(oPropertyFile, oPrefix+p.first, p.second);

	for(QPair<QString, QTableView *> &p : mTables)
		storeTable(oPropertyFile, oPrefix+p.first, p.second);
}

void RestorableWindow::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	for(QPair<QString, QWidget *> &p : mWidgets)
		restoreWidget(oPropertyFile, oPrefix+p.first, p.second);

	for(QPair<QString, QSplitter *> &p : mSplitters)
		restoreSplitter(oPropertyFile, oPrefix+p.first, p.second);

	for(QPair<QString, QTableView *> &p : mTables)
		restoreTable(oPropertyFile, oPrefix+p.first, p.second);
}

void RestorableWindow::addWidget(QString const &oPropertyName, QWidget *oWidget)
{
	QPair<QString, QWidget *> p(oPropertyName, oWidget);
	mWidgets.append(p);
}

void RestorableWindow::storeWidget(QSettings &oPropertyFile, QString const &oPropertyName, QWidget *oWidget)
{
	oPropertyFile.setValue(oPropertyName+"_maximized", oWidget->isMaximized());
	if(!oWidget->isMaximized())
	{
		oPropertyFile.setValue(oPropertyName+"_pos", oWidget->pos());
		oPropertyFile.setValue(oPropertyName+"_size", oWidget->size());
	}
}

void RestorableWindow::restoreWidget(QSettings &oPropertyFile, QString const &oPropertyName, QWidget *oWidget)
{
	QDesktopWidget desktop;

	int desktopHeight = desktop.geometry().height();
	int desktopWidth = desktop.geometry().width();

	int tst = -1;
	tst = oPropertyFile.value(oPropertyName+"_pos", "-1").toInt();
	if(tst != -1)
	{
		QPoint p = oPropertyFile.value(oPropertyName+"_pos", oWidget->pos()).toPoint();
		if(p.x() > desktopWidth)
			p.setX(0);

		if(p.y() > desktopHeight)
			p.setY(0);
		oWidget->move(p);
	}

	tst = oPropertyFile.value(oPropertyName+"_size", "-1").toInt();
	if(tst != -1)
		oWidget->resize(oPropertyFile.value(oPropertyName+"_size", oWidget->size()).toSize());

	if(oPropertyFile.value(oPropertyName+"_maximized", oWidget->isMaximized()).toBool())
		oWidget->showMaximized();
}

void RestorableWindow::addSplitter(QString const &oPropertyName, QSplitter *oSplitter)
{
	QPair<QString, QSplitter *> p(oPropertyName, oSplitter);
	mSplitters.append(p);
}

void RestorableWindow::storeSplitter(QSettings &oPropertyFile, QString const &oPropertyName, QSplitter *oSplitter)
{
	QList<int> sz = oSplitter->sizes();
	oPropertyFile.setValue(oPropertyName+"_0", sz[0]);
	oPropertyFile.setValue(oPropertyName+"_1", sz[1]);
}

void RestorableWindow::restoreSplitter(QSettings &oPropertyFile, QString const &oPropertyName, QSplitter *oSplitter)
{
	QList<int> sz = oSplitter->sizes();
	sz[0] = oPropertyFile.value(oPropertyName+"_0", "50").toInt();
	sz[1] = oPropertyFile.value(oPropertyName+"_1", "50").toInt();
	oSplitter->setSizes(sz);
}

void RestorableWindow::addTable(QString const &oPropertyName, QTableView *oTable)
{
	QPair<QString, QTableView *> p(oPropertyName, oTable);
	mTables.append(p);
}

void RestorableWindow::storeTable(QSettings &oPropertyFile, QString const &oName, QTableView *oTable)
{
	QHeaderView *header = oTable->horizontalHeader();
	int n = header->count();
	QStringList sl;
	QString s;

	// If the last column is autostreched, we don't store the size
	if(header->stretchLastSection())
		n--;
	for(int i = 0; i < n; i++)
		sl << s.setNum(header->sectionSize(i));

	s = sl.join(";");
	oPropertyFile.setValue(oName, s);
}

void RestorableWindow::restoreTable(QSettings &oPropertyFile, QString const &oName, QTableView *oTable)
{
	QString s = oPropertyFile.value(oName).toString();
	QStringList sl = s.split(";");

	QHeaderView *header = oTable->horizontalHeader();
	int n = header->count();
	if(sl.size() < n)
		n = sl.size();

	// If the last column is autostreched, we don't restore the size
	if(header->stretchLastSection())
		n--;

	for(int i = 0; i < n; i++)
	{
		int sz = sl.at(i).toInt();
		if(sz <= 0)
			sz = 30;
		header->resizeSection(i, sz);
	}
}

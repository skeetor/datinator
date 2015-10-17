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

#include "support/unicode/unicode_types.h"

#include <QtWidgets/QHeaderView>
#include <QtWidgets/QDesktopWidget>

#include "support_qt/window/restorable_window.h"
#include "support/helper/string.h"

Q_DECLARE_METATYPE(QList<int>)

RestorableWindow::RestorableWindow(void)
{
}

RestorableWindow::~RestorableWindow(void)
{
}

void RestorableWindow::store(QSettings &oPropertyFile, spt::string::string_t const &oPrefix)
{
	for(std::pair<spt::string::string_t, QWidget *> &p : mWidgets)
		storeWidget(oPropertyFile, oPrefix+p.first, p.second);

	for(std::pair<spt::string::string_t, QSplitter *> &p : mSplitters)
		storeSplitter(oPropertyFile, oPrefix+p.first, p.second);

	for(std::pair<spt::string::string_t, QTableView *> &p : mTables)
		storeTable(oPropertyFile, oPrefix+p.first, p.second);
}

void RestorableWindow::restore(QSettings &oPropertyFile, spt::string::string_t const &oPrefix)
{
	for(std::pair<spt::string::string_t, QWidget *> &p : mWidgets)
		restoreWidget(oPropertyFile, oPrefix+p.first, p.second);

	for(std::pair<spt::string::string_t, QSplitter *> &p : mSplitters)
		restoreSplitter(oPropertyFile, oPrefix+p.first, p.second);

	for(std::pair<spt::string::string_t, QTableView *> &p : mTables)
		restoreTable(oPropertyFile, oPrefix+p.first, p.second);
}

void RestorableWindow::addWidget(spt::string::string_t const &oPropertyName, QWidget *oWidget)
{
	std::pair<spt::string::string_t, QWidget *> p(oPropertyName, oWidget);
	mWidgets.push_back(p);
}

void RestorableWindow::storeWidget(QSettings &oPropertyFile, spt::string::string_t const &oPropertyName, QWidget *oWidget)
{
	auto property = spt::string::toQt(oPropertyName);

	oPropertyFile.setValue(property+"_maximized", oWidget->isMaximized());
	if(!oWidget->isMaximized())
	{
		oPropertyFile.setValue(property+"_pos", oWidget->pos());
		oPropertyFile.setValue(property+"_size", oWidget->size());
	}
}

void RestorableWindow::restoreWidget(QSettings &oPropertyFile, spt::string::string_t const &oPropertyName, QWidget *oWidget)
{
	auto property = spt::string::toQt(oPropertyName);
	QDesktopWidget desktop;

	int desktopHeight = desktop.geometry().height();
	int desktopWidth = desktop.geometry().width();

	int tst = -1;
	tst = oPropertyFile.value(property+"_pos", "-1").toInt();
	if(tst != -1)
	{
		QPoint p = oPropertyFile.value(property+"_pos", oWidget->pos()).toPoint();
		if(p.x() > desktopWidth)
			p.setX(0);

		if(p.y() > desktopHeight)
			p.setY(0);
		oWidget->move(p);
	}

	tst = oPropertyFile.value(property+"_size", "-1").toInt();
	if(tst != -1)
		oWidget->resize(oPropertyFile.value(property+"_size", oWidget->size()).toSize());

	if(oPropertyFile.value(property+"_maximized", oWidget->isMaximized()).toBool())
		oWidget->showMaximized();
}

void RestorableWindow::addSplitter(spt::string::string_t const &oPropertyName, QSplitter *oSplitter)
{
	std::pair<spt::string::string_t, QSplitter *> p(oPropertyName, oSplitter);
	mSplitters.push_back(p);
}

void RestorableWindow::storeSplitter(QSettings &oPropertyFile, spt::string::string_t const &oPropertyName, QSplitter *oSplitter)
{
	auto property = spt::string::toQt(oPropertyName);
	QList<int> sz = oSplitter->sizes();

	oPropertyFile.setValue(property+"_0", sz[0]);
	oPropertyFile.setValue(property+"_1", sz[1]);
}

void RestorableWindow::restoreSplitter(QSettings &oPropertyFile, spt::string::string_t const &oPropertyName, QSplitter *oSplitter)
{
	auto property = spt::string::toQt(oPropertyName);
	QList<int> sz = oSplitter->sizes();

	sz[0] = oPropertyFile.value(property+"_0", "50").toInt();
	sz[1] = oPropertyFile.value(property+"_1", "50").toInt();
	oSplitter->setSizes(sz);
}

void RestorableWindow::addTable(spt::string::string_t const &oPropertyName, QTableView *oTable)
{
	std::pair<spt::string::string_t, QTableView *> p(oPropertyName, oTable);
	mTables.push_back(p);
}

void RestorableWindow::storeTable(QSettings &oPropertyFile, spt::string::string_t const &oName, QTableView *oTable)
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
	oPropertyFile.setValue(spt::string::toQt(oName), s);
}

void RestorableWindow::restoreTable(QSettings &oPropertyFile, spt::string::string_t const &oName, QTableView *oTable)
{
	QString s = oPropertyFile.value(spt::string::toQt(oName)).toString();
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

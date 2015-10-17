/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * RestorableWidgets (c) by Gerhard W. Gruber in 2014
 *
 * A helper class to save and restore windows to it's current size
 * and position. It also includes support for specific controls.
 *
 ******************************************************************************/

#ifndef RESTORABLE_WINDOW_H
#define RESTORABLE_WINDOW_H

#include "support/unicode/unicode_types.h"

#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <utility>
#include <vector>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>

#include "support_qt/support_qt_dll_api.h"

/**
 * RestorableWidgets is a helper class which stores a window and
 * certain associated widgets to save and restore the characteristics
 * into a propertyfile.
 * The name passed into the public functions is the variable name which should
 * be used in the property file. The prefix is a an optional name like a group.
 * In the protected functions, the full name needs to be passed in.
 *
 * Example:
 * RestorableWidgets("myWidgets", pMyWidgets);
 * addSplitter("FirstSplitter", pSplitter);
 * addSplitter("SecondSplitter", pSplitter);
 * store(PropertyFile, "main/");
 *
 * This would save the characteristics of a window with the name "myWidgets" as
 * well as adding two splitters. When the objects are saved, they will be saved
 * under the group "main" with their respective names.
 *
 * When the objects are restored, the window will get it's original size and
 * position including the maximized state if appropriate. The splitters will also
 * get their size as they were persisted.
 *
 */
class SUPPORT_QT_DLL_EXPORT RestorableWindow
{
public:
	RestorableWindow(void);
	virtual ~RestorableWindow(void);

	virtual void store(QSettings &oPropertyFile, spt::string::string_t const &oPrefix);
	virtual void restore(QSettings &oPropertyFile, spt::string::string_t const &oPrefix);

	virtual void addWidget(spt::string::string_t const &oPropertyName, QWidget *oWidget);
	virtual void addSplitter(spt::string::string_t const &oPropertyName, QSplitter *oSplitter);
	virtual void addTable(spt::string::string_t const &oPropertyName, QTableView *oTable);

protected:
	virtual void storeWidget(QSettings &oPropertyFile, spt::string::string_t const &oName, QWidget *oWidgets);
	virtual void restoreWidget(QSettings &oPropertyFile, spt::string::string_t const &oName, QWidget *oWidgets);

	virtual void storeSplitter(QSettings &oPropertyFile, spt::string::string_t const &oName, QSplitter *oSplitter);
	virtual void restoreSplitter(QSettings &oPropertyFile, spt::string::string_t const &oName, QSplitter *oSplitter);

	virtual void storeTable(QSettings &oPropertyFile, spt::string::string_t const &oName, QTableView *oTable);
	virtual void restoreTable(QSettings &oPropertyFile, spt::string::string_t const &oName, QTableView *oTable);

private:
	std::vector<std::pair<spt::string::string_t, QWidget *>> mWidgets;
	std::vector<std::pair<spt::string::string_t, QSplitter *>> mSplitters;
	std::vector<std::pair<spt::string::string_t, QTableView *>> mTables;
};

#endif // RESTORABLE_WINDOW_H

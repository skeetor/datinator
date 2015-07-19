/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef DBCOLUMN_OBSERVER_H_INCLUDED
#define DBCOLUMN_OBSERVER_H_INCLUDED

#include <QtCore/QList>

#include <support/patterns/observer.h>
#include <support/db/dbcolumn.h>
#include "plugins/datinator_types.h"

class DBColumnObserver
	: public Dispatcher<QList<DatabaseColumn *> * /* pColumnList */>
{
public:
	DBColumnObserver(void);
	DBColumnObserver(QList<DatabaseColumn *> const &oColumns);
	virtual ~DBColumnObserver(void);

	void setDefault(bool bDefault = true);
	bool isDefault(void);

	QList<DatabaseColumn *> const &getColumns(void) const;
	void setColumns(QList<DatabaseColumn *> const &oColumns);
	void setColumns(QList<DatabaseColumn *> const *oColumns);
	QList<DatabaseColumn *> const &list(void);
	operator QList<DatabaseColumn *> const &(void);
	operator QList<DatabaseColumn *> const &(void) const;
	DBColumnObserver &operator=(QList<DatabaseColumn *> const &oColumns);

	/**
	 * AddColumn appends a new column. If the column already exists it will be ignored.
	 * addColumn does not notify the observers, so if this is desired, it should be done
	 * manually after finished adding all columns.
	 */
	int addColumn(DatabaseColumn *oColumn, bool bCopy = true);

	DatabaseColumn *operator[](int nIndex);
	DatabaseColumn *at(int nIndex);

	/**
	 * Return the index of the specified column.
	 */
	int find(DatabaseColumn const *oColumn) const;

	/**
	 * Normally the class only keeps a shallow copy of the list.
	 * If it is desired to create a deep copy, then copy() will
	 * do this.
	 */
	DBColumnObserver &copy(QList<DatabaseColumn *> const &oColumns, bool bDeepCopy = true);

	/**
	 * Notify the observers with the current or new list of columns and their current selection.
	 * If the selection contains an invalid index it will be set to empty.
	 */
	void notifyColumnListeners(void);

protected:
	void free(void);

private:
	QList<DatabaseColumn *> mColumns;
	bool mAllocated;
	bool mDefault;
};


#endif // DBCOLUMN_OBSERVER_H_INCLUDED

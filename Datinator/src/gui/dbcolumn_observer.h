/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef _DBCOLUMN_OBSERVER_H_INCLUDED
#define _DBCOLUMN_OBSERVER_H_INCLUDED

#include <support/patterns/observer.h>
#include <support/db/dbcolumn.h>
#include "plugins/datinator_types.h"

class DBColumnObserver
	: public Dispatcher<std::vector<DatabaseColumn *> * /* pColumnList */>
{
public:
	DBColumnObserver(void);
	DBColumnObserver(std::vector<DatabaseColumn *> const &oColumns);
	virtual ~DBColumnObserver(void);

	void setDefault(bool bDefault = true);
	bool isDefault(void);

	std::vector<DatabaseColumn *> const &getColumns(void) const;
	void setColumns(std::vector<DatabaseColumn *> const &oColumns);
	void setColumns(std::vector<DatabaseColumn *> const *oColumns);
	std::vector<DatabaseColumn *> const &list(void);
	operator std::vector<DatabaseColumn *> const &(void);
	operator std::vector<DatabaseColumn *> const &(void) const;
	DBColumnObserver &operator=(std::vector<DatabaseColumn *> const &oColumns);

	/**
	 * AddColumn appends a new column. If the column already exists it will be ignored.
	 * addColumn does not notify the observers, so if this is desired, it should be done
	 * manually after finished adding all columns.
	 */
	ssize_t addColumn(DatabaseColumn *oColumn, bool bCopy = true);

	DatabaseColumn *operator[](ssize_t nIndex);
	DatabaseColumn *at(ssize_t nIndex);

	/**
	 * Return the index of the specified column.
	 */
	ssize_t find(DatabaseColumn const *oColumn) const;

	/**
	 * Normally the class only keeps a shallow copy of the list.
	 * If it is desired to create a deep copy, then copy() will
	 * do this.
	 */
	DBColumnObserver &copy(std::vector<DatabaseColumn *> const &oColumns, bool bDeepCopy = true);

	/**
	 * Notify the observers with the current or new list of columns and their current selection.
	 * If the selection contains an invalid index it will be set to empty.
	 */
	void notifyColumnListeners(void);

protected:
	void free(void);

private:
	std::vector<DatabaseColumn *> mColumns;
	bool mAllocated;
	bool mDefault;
};


#endif // _DBCOLUMN_OBSERVER_H_INCLUDED

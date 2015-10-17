/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "gui/dbcolumn_observer.h"

DBColumnObserver::DBColumnObserver(void)
{
	mAllocated = false;
	mDefault = false;
	mColumns.clear();
}

DBColumnObserver::DBColumnObserver(std::vector<DatabaseColumn *> const &oColumns)
{
	mAllocated = false;
	mDefault = false;
	setColumns(oColumns);
}

DBColumnObserver::~DBColumnObserver(void)
{
	free();
}

void DBColumnObserver::setDefault(bool bDefault)
{
	mDefault = bDefault;
}

bool DBColumnObserver::isDefault(void)
{
	return mDefault;
}

std::vector<DatabaseColumn *> const &DBColumnObserver::getColumns(void) const
{
	return mColumns;
}

void DBColumnObserver::setColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	setColumns(&oColumns);
}

void DBColumnObserver::setColumns(std::vector<DatabaseColumn *> const *oColumns)
{
	free();
	if(oColumns)
		copy(*oColumns, false);

	notifyColumnListeners();
}

ssize_t DBColumnObserver::addColumn(DatabaseColumn *oColumn, bool bCopy)
{
	if(!oColumn)
		return -1;

	ssize_t i = -1;
	for(DatabaseColumn * const &col : mColumns)
	{
		i++;
		if(col == oColumn)
			return i;
	}

	if(bCopy)
		oColumn = oColumn->duplicate();

	mColumns.push_back(oColumn);

	return mColumns.size()-1;
}

std::vector<DatabaseColumn *> const &DBColumnObserver::list(void)
{
	return mColumns;
}

DBColumnObserver::operator std::vector<DatabaseColumn *> const &(void)
{
	return mColumns;
}

DBColumnObserver::operator std::vector<DatabaseColumn *> const &(void) const
{
	return mColumns;
}

DBColumnObserver &DBColumnObserver::operator=(std::vector<DatabaseColumn *> const &oColumns)
{
	setColumns(oColumns);
	return *this;
}

/**
 * Normally the class only keeps a shallow copy of the list.
 * If it is desired to create a deep copy, then copy() will
 * do this.
 */
DBColumnObserver &DBColumnObserver::copy(std::vector<DatabaseColumn *> const &oColumns, bool bDeepCopy)
{
	free();
	if(bDeepCopy)
		mAllocated = true;
	else
		mAllocated = false;

	for(DatabaseColumn * const &c : oColumns)
	{
		DatabaseColumn *col = c;
		if(bDeepCopy)
			col = col->duplicate();

		mColumns.push_back(col);
	}

	notifyColumnListeners();
	return *this;
}

void DBColumnObserver::free(void)
{
	if(mAllocated)
	{
#warning TODO: memory leak, how to properly delete the columns
//		for(DatabaseColumn * &col : mColumns)
//			delete col;
	}
	mColumns.clear();
}

void DBColumnObserver::notifyColumnListeners(void)
{
	Dispatcher<std::vector<DatabaseColumn *> *>::notify(&mColumns);
}

DatabaseColumn *DBColumnObserver::at(ssize_t nIndex)
{
	if(static_cast<size_t>(nIndex) > mColumns.size())
		return NULL;

	return mColumns[nIndex];
}

DatabaseColumn *DBColumnObserver::operator[](ssize_t nIndex)
{
	return at(nIndex);
}

ssize_t DBColumnObserver::find(DatabaseColumn const *oColumn) const
{
	if(!oColumn)
		return -1;

	int i = -1;
	for(DatabaseColumn * const &col : mColumns)
	{
		i++;
		if(!col)
			continue;

		if(oColumn == col)
			return i;

		if(oColumn->getName() == col->getName())
			return i;
	}

	return -1;
}

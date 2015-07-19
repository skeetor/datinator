/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef IDATACONTAINERWRITER_H
#define IDATACONTAINERWRITER_H

#include "idata_container.h"

class IDataContainerWriter : public virtual IDataContainer
{
public:
	IDataContainerWriter(void) {}
	virtual ~IDataContainerWriter(void) {}

	/**
	 * The data container should return true if it is able to change,
	 * add or remove columns.
	 */
	virtual bool canModifyColumns(void) const = 0;
	virtual bool canTruncate(void) const = 0;

	/**
	 * If it returns true, the default mode will be to switch to truncate,
	 * otherwise append mode is selected.
	 */
	virtual bool defaultTruncate(void) const = 0;

	/**
	 * Set to true if the truncate option is selected. If the writer doesn't support
	 * it, it should be ignored.
	 */
	virtual void setTruncateMode(bool bTruncate = true) = 0;

	virtual bool prepareOpen(QList<DatabaseColumn *> const &oColumns) = 0;

	virtual int write(QList<DatabaseColumn *> const &oColumns, QList<QString> const &oRow) = 0;
	virtual void commit(void) = 0;
	virtual void rollback(void) = 0;
};

#endif // IDATACONTAINERWRITER_H

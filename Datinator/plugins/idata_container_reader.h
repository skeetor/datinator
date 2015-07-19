/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef IDATACONTAINERREADER_H
#define IDATACONTAINERREADER_H

#include "idata_container.h"

class IDataContainerReader : virtual public IDataContainer
{
public:
	IDataContainerReader(void) {}
	virtual ~IDataContainerReader(void) {}

	/**
	 * Return < 0 means an error. If return == 0, it means EOF. A positive value
	 * is the number of rows returned and has to be 1 here. The number of elements
	 * in the row, must match the number of columns in the getColumns()
	 */
	virtual int read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow) = 0;

	/**
	 * Return -1 if the operation is not supported, otherwise the number of rows for the source.
	 */
	virtual int count(void) = 0;
};

#endif // IDATACONTAINERREADER_H

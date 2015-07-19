/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QString>

#include <support/support_defs.h>
#include "plugin/container/reader_base.h"

ReaderBase::ReaderBase(void)
{
}

ReaderBase::~ReaderBase(void)
{
}

int ReaderBase::read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow)
{
	UNUSED(oColumns);
	UNUSED(oRow);

	return -1;
}

int ReaderBase::count(void)
{
	return -1;
}

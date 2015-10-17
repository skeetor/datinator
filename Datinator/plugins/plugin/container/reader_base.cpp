/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <support/support_defs.h>
#include "datinator_types.h"
#include "plugin/container/reader_base.h"

ReaderBase::ReaderBase(void)
{
}

ReaderBase::~ReaderBase(void)
{
}

int ReaderBase::read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow)
{
	UNUSED(oColumns);
	UNUSED(oRow);

	return -1;
}

int ReaderBase::count(void)
{
	return -1;
}

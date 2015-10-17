/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "plugin/container/writer_base.h"

WriterBase::WriterBase(void)
{
	mTruncate = false;
}

WriterBase::~WriterBase(void)
{
}

void WriterBase::setTruncateMode(bool bTruncate)
{
	mTruncate = bTruncate;
}

bool WriterBase::truncateMode(void)
{
	return mTruncate;
}

bool WriterBase::defaultTruncate(void) const
{
	return true;
}

bool WriterBase::prepareOpen(std::vector<DatabaseColumn *> const &oColumns)
{
	return prepareOpen(oColumns);
}

int WriterBase::write(std::vector<DatabaseColumn *> const &oColumns, std::vector<StdString> const &oRow)
{
	return write(oColumns, oRow);
}

void WriterBase::commit(void)
{
}

void WriterBase::rollback(void)
{
}

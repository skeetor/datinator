/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef READER_BASE_H_INCLUDED
#define READER_BASE_H_INCLUDED

#include "plugin/plugin_dll_api.h"
#include "idata_container_reader.h"

class PLUGIN_DLL_EXPORT ReaderBase
	: public virtual IDataContainerReader
{
public:
	ReaderBase(void);
	virtual ~ReaderBase(void);

	int read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow) override;
	int count(void) override;
};

#endif // WRITER_BASE_H_INCLUDED

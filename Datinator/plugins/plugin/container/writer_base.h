/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef WRITER_BASE_H_INCLUDED
#define WRITER_BASE_H_INCLUDED

#include "plugin/plugin_dll_api.h"
#include "idata_container_writer.h"

class PLUGIN_DLL_EXPORT WriterBase
	: public virtual IDataContainerWriter
{
public:
	WriterBase(void);
	virtual ~WriterBase(void);

public: // IDataContainer
	void setTruncateMode(bool bTruncate = true) override;
	bool defaultTruncate(void) const override;
	bool prepareOpen(std::vector<DatabaseColumn *> const &oColumns) override;

	int	write(std::vector<DatabaseColumn *> const &oColumns, std::vector<StdString> const &oRow) override;

	void commit(void) override;
	void rollback(void) override;

protected:
	bool truncateMode(void);

private:
	bool mTruncate;
};

#endif // WRITER_BASE_H_INCLUDED

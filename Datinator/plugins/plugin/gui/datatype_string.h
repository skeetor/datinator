/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef DATATYPE_STRING_H_INCLUDED
#define DATATYPE_STRING_H_INCLUDED

#include <vector>
#include <utility>

#include "datinator_types.h"
#include "plugin/plugin_dll_api.h"
#include "support/db/dbcolumn.h"

class PLUGIN_DLL_EXPORT TypeString
{
public:
	TypeString(void);
	virtual ~TypeString(void);

	StdString toString(spt::db::DataType nDatatype) const;
	spt::db::DataType toType(StdString const &oTypename) const;

	std::vector<std::pair<spt::db::DataType, StdString>> items(void) const;
	std::vector<StdString> strings(void) const;
	std::vector<spt::db::DataType> types(void) const;

protected:
	void initTypes(void);
	std::pair<spt::db::DataType, StdString> itemAt(ssize_t nIndex) const;
	ssize_t typeIndex(spt::db::DataType nDatatype) const;
	ssize_t stringIndex(StdString const &oTypename) const;

private:
	std::vector<std::pair<spt::db::DataType, StdString>> mTypeStrings;
};

extern TypeString PLUGIN_DLL_EXPORT TypeStringProvider;

#endif // DATATYPE_STRING_H_INCLUDED

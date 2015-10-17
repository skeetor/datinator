/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "plugin/gui/datatype_string.h"

TypeString PLUGIN_DLL_EXPORT TypeStringProvider;

TypeString::TypeString(void)
{
	initTypes();
}

TypeString::~TypeString(void)
{
}

void TypeString::initTypes(void)
{
	std::pair<spt::db::DataType, StdString> p;

	p.first = spt::db::type_unknown;
	p.second = "Undefined";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_string;
	p.second = "String";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_date;
	p.second = "Date";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_time;
	p.second = "Time";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_time_long;
	p.second = "Time with miliseconds";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_datetime;
	p.second = "Datetime";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_datetime_long;
	p.second = "Datetime with miliseconds";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_integer;
	p.second = "Integer";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_decimal;
	p.second = "Decimal";
	mTypeStrings.push_back(p);

	p.first = spt::db::type_ip;
	p.second = "IP adress";
	mTypeStrings.push_back(p);
}

std::pair<spt::db::DataType, StdString> TypeString::itemAt(ssize_t nIndex) const
{
	std::pair<spt::db::DataType, StdString>p;

	if(nIndex == -1 || nIndex >= (size_t)mTypeStrings.size())
		nIndex = typeIndex(spt::db::DataType::type_unknown);

	p = mTypeStrings.at(nIndex);

	return p;
}

ssize_t TypeString::typeIndex(spt::db::DataType nDatatype) const
{
	int i = 0;
	for(std::pair<spt::db::DataType, StdString> const &p : mTypeStrings)
	{
		if(p.first == nDatatype)
			return i;

		i++;
	}

	return -1;
}

ssize_t TypeString::stringIndex(StdString const &oTypename) const
{
	ssize_t i = 0;
	for(std::pair<spt::db::DataType, StdString> const &p : mTypeStrings)
	{
		if(p.second == oTypename)
			return i;

		i++;
	}

	return -1;
}

spt::db::DataType TypeString::toType(StdString const &oTypename) const
{
	return itemAt(stringIndex(oTypename)).first;
}


StdString TypeString::toString(spt::db::DataType nDataType) const
{
	return itemAt(typeIndex(nDataType)).second;
}

std::vector<std::pair<spt::db::DataType, StdString>> TypeString::items(void) const
{
	return mTypeStrings;
}

std::vector<StdString> TypeString::strings(void) const
{
	std::vector<StdString>l;
	for(std::pair<spt::db::DataType, StdString> const &item : mTypeStrings)
		l.push_back(item.second);

	return l;
}

std::vector<spt::db::DataType> TypeString::types(void) const
{
	std::vector<spt::db::DataType>l;
	for(std::pair<spt::db::DataType, StdString> const &item : mTypeStrings)
		l.push_back(item.first);

	return l;
}

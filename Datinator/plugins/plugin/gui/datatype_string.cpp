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
	QPair<supportlib::db::DataType, QString> p;

	p.first = supportlib::db::type_unknown;
	p.second = "Undefined";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_string;
	p.second = "String";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_date;
	p.second = "Date";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_time;
	p.second = "Time";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_time_long;
	p.second = "Time with miliseconds";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_datetime;
	p.second = "Datetime";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_datetime_long;
	p.second = "Datetime with miliseconds";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_integer;
	p.second = "Integer";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_decimal;
	p.second = "Decimal";
	mTypeStrings.append(p);

	p.first = supportlib::db::type_ip;
	p.second = "IP adress";
	mTypeStrings.append(p);
}

QPair<supportlib::db::DataType, QString> TypeString::itemAt(int nIndex) const
{
	QPair<supportlib::db::DataType, QString>p;

	if(nIndex == -1 || nIndex >= mTypeStrings.size())
		nIndex = typeIndex(supportlib::db::DataType::type_unknown);

	p = mTypeStrings.at(nIndex);

	return p;
}

int TypeString::typeIndex(supportlib::db::DataType nDatatype) const
{
	int i = 0;
	for(QPair<supportlib::db::DataType, QString> const &p : mTypeStrings)
	{
		if(p.first == nDatatype)
			return i;

		i++;
	}

	return -1;
}

int TypeString::stringIndex(QString const &oTypename) const
{
	int i = 0;
	for(QPair<supportlib::db::DataType, QString> const &p : mTypeStrings)
	{
		if(p.second == oTypename)
			return i;

		i++;
	}

	return -1;
}

supportlib::db::DataType TypeString::toType(QString const &oTypename) const
{
	return itemAt(stringIndex(oTypename)).first;
}


QString TypeString::toString(supportlib::db::DataType nDataType) const
{
	return itemAt(typeIndex(nDataType)).second;
}

QList<QPair<supportlib::db::DataType, QString>> TypeString::items(void) const
{
	return mTypeStrings;
}

QList<QString> TypeString::strings(void) const
{
	QList<QString>l;
	for(QPair<supportlib::db::DataType, QString> const &item : mTypeStrings)
		l.append(item.second);

	return l;
}

QList<supportlib::db::DataType> TypeString::types(void) const
{
	QList<supportlib::db::DataType>l;
	for(QPair<supportlib::db::DataType, QString> const &item : mTypeStrings)
		l.append(item.first);

	return l;
}

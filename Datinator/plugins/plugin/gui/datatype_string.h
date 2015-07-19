/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef DATATYPE_STRING_H_INCLUDED
#define DATATYPE_STRING_H_INCLUDED

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPair>

#include "plugin/plugin_dll_api.h"
#include "support/db/dbcolumn.h"

class PLUGIN_DLL_EXPORT TypeString
{
public:
	TypeString(void);
	virtual ~TypeString(void);

	QString toString(supportlib::db::DataType nDatatype) const;
	supportlib::db::DataType toType(QString const &oTypename) const;

	QList<QPair<supportlib::db::DataType, QString>> items(void) const;
	QList<QString> strings(void) const;
	QList<supportlib::db::DataType> types(void) const;

protected:
	void initTypes(void);
	QPair<supportlib::db::DataType, QString> itemAt(int nIndex) const;
	int typeIndex(supportlib::db::DataType nDatatype) const;
	int stringIndex(QString const &oTypename) const;

private:
	QList<QPair<supportlib::db::DataType, QString>> mTypeStrings;
};

extern TypeString PLUGIN_DLL_EXPORT TypeStringProvider;

#endif // DATATYPE_STRING_H_INCLUDED

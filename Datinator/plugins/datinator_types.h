/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef DATINATOR_TYPES_H_INCLUDED
#define DATINATOR_TYPES_H_INCLUDED

#include <QtCore/QString>
#include "support/db/dbcolumn.h"
#include "support/unicode/unicode_types.h"

typedef supportlib::db::DBColumn<QString, QString> DatabaseColumn;
typedef supportlib::string::string_t StdString;
typedef supportlib::string::char_t StdChar;

#endif // DATINATOR_TYPES_H_INCLUDED

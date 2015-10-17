/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef _DATINATOR_TYPES_H_INCLUDED
#define _DATINATOR_TYPES_H_INCLUDED

#define USE_STD_LISTENER

#include "support/db/dbcolumn.h"
#include "support/unicode/unicode_types.h"

typedef spt::string::string_t StdString;
typedef spt::string::char_t StdChar;

typedef spt::db::DBColumn<StdString, StdString> DatabaseColumn;

#endif // _DATINATOR_TYPES_H_INCLUDED

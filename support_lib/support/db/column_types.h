/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef COLUMN_TYPES_H
#define COLUMN_TYPES_H

namespace supportlib
{
namespace db
{

typedef enum
{
	type_unknown,
	type_string,
	type_date,
	type_time,
	type_time_long,
	type_datetime,
	type_datetime_long,
	type_integer,
	type_decimal,
	type_ip
} DataType;

} // namespace db
} // namespace supportlib

#endif // COLUMN_TYPES_H

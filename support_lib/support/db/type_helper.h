/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

/*******************************************************************************
 * TYPE_HELPER (c) by Gerhard W. Gruber 2014
 *
 * DESCRIPTION: This module implements functions which try
 * to detect the dataype from a string.
 */

#ifndef TYPE_HELPER_H
#define TYPE_HELPER_H

#include <string>

#include "support/support_dll_api.h"
#include "support/unicode/unicode_types.h"
#include "support/db/column_types.h"

#define BIT_VALUE(bitIndex) 				(1 << bitIndex)
#define BIT_SET(val, bitIndex) 		(val |=  (1 << bitIndex))
#define BIT_CLEAR(val, bitIndex)	(val &= ~(1 << bitIndex))
#define BIT_TOGGLE(val, bitIndex)	(val ^=  (1 << bitIndex))
#define BIT_IS_SET(val, bitIndex)	(val &   (1 << bitIndex))

spt::string::string_t SUPPORT_DLL_EXPORT strip(spt::string::string_t const &oString, bool bFromBegin = true, bool bFromEnd = true);

spt::string::string_t SUPPORT_DLL_EXPORT toTypeString(spt::db::DataType oType);
spt::db::DataType SUPPORT_DLL_EXPORT toType(spt::string::string_t const &oType);
spt::db::DataType SUPPORT_DLL_EXPORT guessType(spt::string::string_t const &oValue);

/**
 * Returns the number of characters converted to a number.
 * If the return value is > 0 then the converted number is returned
 * in nValue.
 */
int isInteger(spt::string::string_t const &oValue);
int toInteger(spt::string::string_t const &oValue, int nIndex, int &nValue);

int isDecimal(spt::string::string_t const &oValue);
int toDecimal(spt::string::string_t const &oValue, int nIndex, double &nValue);

/**
 * Returns true if the date is a valid looking date in the format
 * YYYY.MM.DD, MM.DD.YYYY, MM.DD.YY or any combination thereof. The
 * separating character is required but ignored and can be any non-digit.
 */
int isDate(spt::string::string_t const &oValue);
int isTimeMilliseconds(spt::string::string_t const &oValue);
int isTime(spt::string::string_t const &oValue);
int isDateTime(spt::string::string_t const &oValue);
int isDateTimeMilliseconds(spt::string::string_t const &oValue);

int toDate(spt::string::string_t const &oValue, int *oYear = NULL, int *oMonth = NULL, int *oDay = NULL);
int toTime(spt::string::string_t const &oValue, int *oHour = NULL, int *oMinute = NULL, int *oSecond = NULL, bool *bMilis = NULL, double *oMilis = NULL);

int isIP(spt::string::string_t const &oValue);
int toIP(spt::string::string_t const &oValue, int *oAddress = NULL);

#endif // HEADER_TYPES_H

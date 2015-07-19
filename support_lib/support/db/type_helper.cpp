/*******************************************************************************
 *
 * Support Library (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

/**************************************************************
 * TypeHelper (c) by Gerhard W. Gruber 2014
 *
 * DESCRIPTION: This module contains an assortment
 * of functions which help to determine the datatype
 * from a string.
 */

#include <iostream>
#include <sstream>
#include <cstring>

#include "type_helper.h"

#define BIT_STRING						0
#define BIT_DATETIME_LONG				1
#define BIT_TIME_LONG					2
#define BIT_DATETIME					3
#define BIT_DATE						4
#define BIT_TIME						5
#define BIT_DECIMAL						6
#define BIT_INTEGER						7
#define BIT_IP							8

#define BIT_DATETIME_MASK	((1 << BIT_DATE) + (1 << BIT_TIME))

typedef struct
{
	unsigned int mBitNumber;
    int (*mEvaluate)(supportlib::string::string_t const &oValue);
} TypeEvaluator;


TypeEvaluator Evaluators[] =
{
	// These are not needed here, because we tokenize the string anyway
	// so the test function will never see a complete string.
//	{ BIT_DATETIME_LONG, isDateTimeMilliseconds},
//	{ BIT_DATETIME, isDateTime},
	{ BIT_DATE, isDate},
	{ BIT_TIME_LONG, isTimeMilliseconds},
	{ BIT_TIME, isTime},
	{ BIT_DECIMAL, isDecimal},
	{ BIT_INTEGER, isInteger},
	{ BIT_IP, isIP},
	{ 0, NULL}
};

supportlib::string::string_t toTypeString(supportlib::db::DataType oType)
{
	supportlib::string::string_t s = "type_unknown";

	switch(oType)
	{
		case supportlib::db::type_date:
			s = "type_date";
		break;

		case supportlib::db::type_time:
			s = "type_time";
		break;

		case supportlib::db::type_time_long:
			s = "type_time_long";
		break;

		case supportlib::db::type_datetime:
			s = "type_datetime";
		break;

		case supportlib::db::type_datetime_long:
			s = "type_datetime_long";
		break;

		case supportlib::db::type_string:
			s = "type_string";
		break;

		case supportlib::db::type_decimal:
			s = "type_decimal";
		break;

		case supportlib::db::type_integer:
			s = "type_integer";
		break;

		case supportlib::db::type_ip:
			s = "type_ip";
		break;

		default:
			break;
	}

	return s;
}

supportlib::db::DataType toType(supportlib::string::string_t const &oType)
{
	supportlib::db::DataType rc = supportlib::db::DataType::type_unknown;

	if(oType == "type_date")
		rc = supportlib::db::DataType::type_date;
	else if(oType == "type_time")
		rc = supportlib::db::DataType::type_time;
	else if(oType == "type_time_long")
		rc = supportlib::db::DataType::type_time_long;
	else if(oType == "type_datetime")
		rc = supportlib::db::DataType::type_datetime;
	else if(oType == "type_datetime_long")
		rc = supportlib::db::DataType::type_datetime_long;
	else if(oType == "type_string")
		rc = supportlib::db::DataType::type_string;
	else if(oType == "type_decimal")
		rc = supportlib::db::DataType::type_decimal;
	else if(oType == "type_integer")
		rc = supportlib::db::DataType::type_integer;
	else if(oType == "type_ip")
		rc = supportlib::db::DataType::type_ip;

	return rc;
}

supportlib::db::DataType guessType(supportlib::string::string_t const &oValue)
{
	supportlib::string::string_t v = strip(oValue);
	int len = v.length();
	if(len == 0)
		return supportlib::db::DataType::type_unknown;

	supportlib::db::DataType result = supportlib::db::DataType::type_unknown;
	unsigned int type_mask = 0;
	int tokens = 0;
	int n;
	int i = 0;

	supportlib::string::string_t prefix;
	supportlib::string::stringstream_t ss(oValue);

	while(ss.rdbuf()->in_avail() > 0)
	{
		i = 0;
		TypeEvaluator *eval = &Evaluators[i];

		ss >> v;
		v = prefix + v;

		if(v.length() == 0)
			break;

		if(v == "+" || v == "-")
		{
			prefix = v;
			continue;
		}

		prefix.clear();
		tokens++;
		while(eval->mEvaluate)
		{
			// We expect only one entry per item. If this is not the case
			// we can already say that the format is not determinable anymore
			// and we simply report a string as default.
			if((n = eval->mEvaluate(v)) > 0)
			{
				if(BIT_IS_SET(type_mask, eval->mBitNumber))
					return supportlib::db::DataType::type_string;

				BIT_SET(type_mask, eval->mBitNumber);

				v = v.substr(n);
				if(v.length() == 0)
					break;

				i = 0;
			}
			else
				i++;

			eval = &Evaluators[i];
		}
	}

	// If datetime long is set, we can clear all the other bits as they don't contribute anything.
	if(type_mask & BIT_VALUE(BIT_DATETIME_LONG))
		type_mask &= ~((1 << BIT_DATETIME) | (1 << BIT_DATE) | (1 << BIT_TIME) | (1 << BIT_TIME_LONG));

	if(type_mask & BIT_VALUE(BIT_TIME_LONG))
	{
		// If one of the date bits is set, it is a long datetime format.
		if(type_mask & ((1 << BIT_DATE) | (1 << BIT_DATETIME) | (1 << BIT_DATETIME_LONG)))
		{
			BIT_SET(type_mask, BIT_DATETIME_LONG);
			type_mask &= ~((1 << BIT_DATE) | (1 << BIT_TIME) | (1 << BIT_DATETIME) | (1 << BIT_TIME_LONG));
		}
		else
			type_mask &= ~(1 << BIT_TIME);
	}

	// If a LONG version was found this will not trigger anymore as it should be cleared already.
	if(type_mask & BIT_VALUE(BIT_DATETIME))
		type_mask &= ~((1 << BIT_DATETIME_LONG) | (1 << BIT_DATE) | (1 << BIT_TIME) | (1 << BIT_TIME_LONG));

	if(type_mask & (1 << BIT_DATE))
	{
		if(type_mask & (1 << BIT_TIME))
		{
			BIT_SET(type_mask, BIT_DATETIME);
			type_mask &= ~((1 << BIT_DATE) | (1 << BIT_TIME) | (1 << BIT_TIME_LONG));
		}
	}

	switch(type_mask)
	{
		case BIT_VALUE(BIT_DATETIME):
			result = supportlib::db::DataType::type_datetime;
			tokens--;
		break;

		case BIT_VALUE(BIT_DATETIME_LONG):
		case BIT_VALUE(BIT_DATE) + BIT_VALUE(BIT_TIME_LONG):
		case BIT_VALUE(BIT_DATE) + BIT_VALUE(BIT_TIME) + BIT_VALUE(BIT_TIME_LONG):
			result = supportlib::db::DataType::type_datetime_long;
			tokens--;
		break;

		case BIT_VALUE(BIT_DATE):
			result = supportlib::db::DataType::type_date;
		break;

		case BIT_VALUE(BIT_TIME):
			result = supportlib::db::DataType::type_time;
		break;

		case BIT_VALUE(BIT_INTEGER):
			result = supportlib::db::DataType::type_integer;
		break;

		case BIT_VALUE(BIT_DECIMAL):
			result = supportlib::db::DataType::type_decimal;
		break;

		case BIT_VALUE(BIT_IP):
			result = supportlib::db::DataType::type_ip;
		break;

		default:
			result = supportlib::db::DataType::type_string;
		break;
	}

	if(tokens != 1)
		result = supportlib::db::DataType::type_string;

	return result;
}

supportlib::string::string_t strip(supportlib::string::string_t const &oString, bool bBegin, bool bEnd)
{
	int start = 0;
	int end = oString.length();

	if(start == end)
		return oString;

	supportlib::string::string_t s(oString);
	if(bBegin)
	{
		while(start < end)
		{
			char c = s[start];
			if(c == ' ' || c == '\t')
				start++;
			else
				break;
		}
	}

	if(bEnd)
	{
		while(end > start)
		{
			char c = s[end-1];
			if(c == ' ' || c == '\t')
				end--;
			else
				break;
		}
	}

	s = s.substr(start, end-start);

	return s;
}

int isDecimal(supportlib::string::string_t const &oValue)
{
	double dummy;
	int n = toDecimal(oValue, 0, dummy);
	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int toDecimal(supportlib::string::string_t const &oValue, int nIndex, double &nValue)
{
	nValue = 0;
	int n = oValue.length();
	if(nIndex >= n)
		return 0;

	int comma = 0;
	int i = nIndex;
	if(i < 0)
		i = 0;

	// Leading sign is allowed.
	int sign = 1;
	if(oValue[i] == '-' || oValue[i] == '+')
	{
		if(oValue[i] == '-')
			sign = -1;

		i++;

		// If the sign is the only char in the string,
		// we report it as non-convertible.
		if(i >= n)
			return 0;
	}

	while(i < n)
	{
		char c = oValue[i];
		if(c >= '0' && c <= '9')
		{
			nValue *= 10;
			nValue += (c-'0');

			comma *= 10;
		}
		else if(c == '.')
		{
			if(comma)
				return 0;

			comma = 1;
		}
		else
		{
			if(comma)
				nValue /= comma;

			nValue *= sign;
			return i;
		}

		i++;
	}

	if(comma)
		nValue /= comma;
	else
		return 0;

	nValue *= sign;
	return i;
}

int isInteger(supportlib::string::string_t const &oValue)
{
	int dummy;

	int n = toInteger(oValue, 0, dummy);
	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int toInteger(supportlib::string::string_t const &oValue, int nIndex, int &nValue)
{
	nValue = 0;
	int n = oValue.length();
	if(nIndex >= n)
		return 0;

	int i = nIndex;
	if(i < 0)
		i = 0;

	// Leading sign is allowed.
	int sign = 1;
	if(oValue[i] == '-' || oValue[i] == '+')
	{
		if(oValue[i] == '-')
			sign = -1;

		i++;

		// If the sign is the only char in the string,
		// we report it as non-convertible.
		if(i >= n)
			return 0;
	}

	while(i < n)
	{
		char c = oValue[i];
		if(c >= '0' && c <= '9')
		{
			nValue *= 10;
			nValue += (c-'0');
		}
		else
		{
			nValue *= sign;
			return i;
		}

		i++;
	}

	nValue *= sign;
	return i;
}

int isTimeMilliseconds(supportlib::string::string_t const &oValue)
{
	bool isMilli;

	int n = toTime(oValue, NULL, NULL, NULL, &isMilli);
	if(isMilli == false)
		return 0;

	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int isTime(supportlib::string::string_t const &oValue)
{
	int n = toTime(oValue);
	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int toTime(supportlib::string::string_t const &oValue, int *oHour, int *oMinute, int *oSecond, bool *bMilis, double *oMilis)
{
	int v[3];
	int i = 0;
	int colon = 0;
	int pos = 0;
	int n = 0;
	bool milis = false;
	double mil = 0;
	char c = 0;

	supportlib::string::string_t s = oValue;

	memset(v, 0, sizeof(v));
	while(1)
	{
		if(i >= 3 && milis == false)
			return 0;

		int val;
		s = strip(s);
		pos = toInteger(s, 0, val);
		if(pos == 0)
			return 0;

		n += pos;
		if(val > 24)
		{
			if(i == 0)
				return 0;

			if(val > 59)
				return 0;
		}

		v[i++] = val;

		c = s[pos];
		if(c == 0)
		{
			if(i > 1 && i < 4)
				break;
			else
				return 0;
		}

		if(c == '.')
		{
			if(colon < 2)
				return 0;

			milis = true;
			break;
		}

		if(c == ':')
		{
			if(colon > 2)
				return 0;

			colon++;
		}
		else
			return 0;

		pos++;
		n++;
		s = s.substr(pos);
	}

	if(c == '.' && milis)
	{
		s = s.substr(pos);
		n += toDecimal(s, 0, mil);
	}

	if(oHour)
		*oHour = v[0];

	if(oMinute)
		*oMinute = v[1];

	if(oSecond)
		*oSecond = v[2];

	if(bMilis)
	{
		*bMilis = milis;
		if(oMilis)
			*oMilis = mil;
	}

    return n;
}


int isIP(supportlib::string::string_t const &oValue)
{
	int n = toIP(oValue);
	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int toIP(supportlib::string::string_t const &oValue, int *oAddress)
{
	int v[4];
	int i = 0;
	int colon = 0;
	int pos;
	int n = 0;

	supportlib::string::string_t s = oValue;

	memset(v, 0, sizeof(v));
	while(1)
	{
		if(i > 3)
			return n;

		int val;
		s = strip(s);
		pos = toInteger(s, 0, val);
		if(pos == 0)
			return 0;

		n += pos;
		if(val > 255)
			return 0;

		v[i] = val;
		char c = s[pos];
		if(c == 0)
			break;

		if(c == '.')
		{
			if(colon >= 3)
				return 0;

			colon++;
		}

		i++;
		pos++;
		n++;
		s = s.substr(pos);
	}

	if(i < 3)
		return 0;

	if(oAddress)
		*oAddress = (v[0] << 24) + (v[1] << 16) + (v[2] << 8) + v[3];

    return n;
}

int isDateTimeMilliseconds(supportlib::string::string_t const &oValue)
{
	int total = toDate(oValue);
	if(!total)
		return 0;

	supportlib::string::string_t val = oValue.substr(total);
	int h;
	int m;
	int s;
	double ms;
	bool bms;
	int n = toTime(val, &h, &m, &s, &bms, &ms);

	if(!n)
		return 0;

	if(!bms)
		return 0;

	n += total;
	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int isDateTime(supportlib::string::string_t const &oValue)
{
	int n = toDate(oValue);
	if(!n)
		return 0;

	supportlib::string::string_t val = oValue.substr(n);
	int h;
	int m;
	int s;
	double ms;
	bool bms;
	n = toTime(val, &h, &m, &s, &bms, &ms);

	if(!n)
		return 0;

	if(bms)
		return 0;

	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int isDate(supportlib::string::string_t const &oValue)
{
	int n = toDate(oValue);
	if(n < static_cast<int>(oValue.length()))
		return 0;

	return n;
}

int toDate(supportlib::string::string_t const &oValue, int *oYear, int *oMonth, int *oDay)
{
	int v[3];
	int i = 0;
	int pos = 0;
	int years = 0;
	int days = 0;
	int n = 0;
	supportlib::string::string_t s = oValue;

	memset(v, 0, sizeof(v));
	while(1)
	{
		if(i >= 3)
			return 0;

		int val;
		s = strip(s);
		pos = toInteger(s, 0, val);
		if(pos == 0)
			return 0;

		n += pos;
		if(val == 0 || val > 31)
		{
			if(years)
				return 0;

			years++;
		}
		else if(val > 12)
		{
			// If the year is between 12 and 31
			// we don't know that it is a year and may be counted
			// as a day. Example "13.02.24"
			if(days && years)
				return 0;

			days++;
		}

		v[i] = val;
		char c = s[pos];
		if(c == 0)
			break;

		if(!(c == '.' || c == '-' || c == '/'))
			return 0;

		i++;
		pos++;
		n++;
		s = s.substr(pos);
	}

	// Put the year in the first position
	if(v[2] > 31)
	{
		int x = v[0];
		v[0] = v[2];
		v[2] = x;
	}

	// Put the day at the last position.
	if(v[1] > 12)
	{
		int x = v[1];
		v[1] = v[2];
		v[2] = x;
	}

	if(v[1] == 0 || v[2] == 0)
		return 0;

	if(oYear)
		*oYear = v[0];

	if(oMonth)
		*oMonth = v[1];

	if(oDay)
		*oDay = v[2];

    return n;
}


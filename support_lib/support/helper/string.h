/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef HELPER_STRING_H
#define HELPER_STRING_H

#include <vector>
#include <sstream>
#include <iomanip>
#include <string>

#include "support/support_dll_api.h"
#include "support/unicode/unicode_types.h"

namespace spt
{
	namespace string
	{
		SUPPORT_DLL_EXPORT std::vector<spt::string::string_t> split(spt::string::string_t const &s, spt::string::char_t delim);
		SUPPORT_DLL_EXPORT std::vector<spt::string::string_t> &split(spt::string::string_t const &s, spt::string::char_t delim, std::vector<spt::string::string_t> &elems);

		/**
		 * maskString replaces the characters '\', '\r', '\t' and '\n'
		 * with the literal characters, so that a string results that can be stored as a single line.
		 */
		SUPPORT_DLL_EXPORT spt::string::string_t maskString(spt::string::string_t const &oInput);

		/**
		 * unmaskString converts a string which was masked with unmaskString() back to it's original
		 * form.
		 */
		SUPPORT_DLL_EXPORT spt::string::string_t unmaskString(spt::string::string_t const &oInput);

		/**
		 * If Qt is available these are some helpers to convert QStrings
		 * from and to std strings.
		 */

		#ifdef QSTRING_H
		inline std::string fromQt(QString const &oString)
		{
			return oString.toStdString();
		}

		inline std::wstring wfromQt(QString const &oString)
		{
			return oString.toStdWString();
		}

		inline QString toQt(std::wstring const &oString)
		{
			return QString::fromStdWString(oString);
		}

		inline QString toQt(std::string const &oString)
		{
			return QString::fromStdString(oString);
		}

		inline std::string toAString(const std::wstring &oString)
		{
			std::string s;
			std::copy(oString.begin(), oString.end(), s.begin());
			return s;
		}
		inline std::wstring toWString(const std::string &oString)
		{
			std::wstring s;
			std::copy(oString.begin(), oString.end(), s.begin());
			return s;
		}

		#ifdef _UNICODE

		inline spt::string::string_t tfromQt(QString const &s) { return wfromQt(s); }

		#else

		inline spt::string::string_t tfromQt(QString const &s) { return fromQt(s); }

		#endif // _UNICODE

		#endif // QSTRING_H

		template <typename T> inline spt::string::string_t toString(T _value)
		{
			spt::string::stringstream_t ss;
			ss << _value;
			spt::string::string_t s;
			ss >> s;
			return s;
		}

		template <typename T> inline std::wstring toWString(T _value)
		{
			std::wstringstream ss;
			ss << _value;
			std::wstring s;
			ss >> s;
			return s;
		}

		template <typename T> inline std::string toAString(T _value)
		{
			std::stringstream ss;
			ss << _value;
			std::string s;
			ss >> s;
			return s;
		}

		template <typename T> inline spt::string::string_t toString(T _value, int nWidth, spt::string::char_t cFiller)
		{
			spt::string::stringstream_t ss;
			ss << std::setfill (cFiller) << std::setw(nWidth) << _value;
			spt::string::string_t s;
			ss >> s;
			return s;
		}

		template <typename T> inline spt::string::string_t toWString(T _value, int nWidth, wchar_t cFiller)
		{
			std::wstringstream ss;
			ss << std::setfill(cFiller) << std::setw(nWidth) << _value;
			std::wstring s;
			ss >> s;
			return s;
		}

		template <typename T> inline spt::string::string_t toAString(T _value, int nWidth, char cFiller)
		{
			std::stringstream ss;
			ss << std::setfill (cFiller) << std::setw(nWidth) << _value;
			std::string s;
			ss >> s;
			return s;
		}
	}
}

#endif // HELPER_STRING_H

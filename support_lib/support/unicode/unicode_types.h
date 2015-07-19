/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

/*
 *  Created on: 27.01.2014
 *      Author: Gerhard.Gruber
 */

#ifndef SUPPORTTYPES_H_
#define SUPPORTTYPES_H_

#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#ifdef _UNICODE
#include <wchar.h>
#endif // _UNICODE

namespace supportlib
{
	namespace string
	{

	inline size_t strlen(const char *s) { return ::strlen(s); }
	inline size_t strlen(const wchar_t *s) { return ::wcslen(s); }
	inline size_t strnlen(const char *s, size_t n) { return ::strnlen(s, n); }
	inline size_t strnlen(const wchar_t *s, size_t n) { return ::wcsnlen(s, n); }

	template <typename T>
	inline size_t strlen_t(T str)
	{
		return supportlib::string::strlen(str);
	}

	template <typename T>
	inline size_t strnlen_t(T str, size_t n)
	{
		return supportlib::string::strnlen(str, n);
	}

	/**
	 * If Qt is available these are some helpers to convert QStrings
	 * from and to std strings.
	 */

	#ifdef QSTRING_H
	inline std::wstring QtStringToWString(QString const &oString)
	{
		return (wchar_t *)oString.unicode();
	}

	inline QString WStringToQtString(std::wstring const &oString)
	{
		return QString::fromWCharArray(oString.c_str());
	}

	inline std::string QtStringToString(QString const &oString)
	{
		//return oString.toUtf8().constData();
		return oString.toLocal8Bit().constData();
	}

	inline QString StringToQtString(std::string const &oString)
	{
		// Note that this may cause problems if the string contains emmbedded 0 bytes.
		// So it only works reliably with standard C strings.
		return QString::fromLocal8Bit(oString.c_str());
	}
	#endif // QSTRING_H

	#ifdef _UNICODE

	#ifdef QSTRING_H

	inline std::wstring QtStringToStringT(QString const &s) { return QtStringToWString(s); }
	inline QString StringTToQtString(std::wstring const &s)	{ return WStringToQtString(s); }

	#endif // QSTRING_H

	typedef wchar_t char_t;
	typedef std::wstring string_t;
	typedef std::wstringstream stringstream_t;
	//typedef std::wofstream ofstream_t;
	//typedef std::wifstream ifstream_t;
	typedef std::wostream ostream_t;

	#define cout_t wcout
	#define cerr_t wcerr
	#define cin_t wcin

	#ifndef _T
	#define _T(x)	(L##x)
	#endif // _T

	#define WIDEN(x) _T(x)
	#define WFILE WIDEN(__FILE__)

	#define snprintf_t	snwprintf
	#define sprintf_t	swprintf
	#define strcpy_t	wcscpy

	#else // _UNICODE

	#ifdef QSTRING_H

	inline std::string QtStringToStringT(QString const &s) { return QtStringToString(s); }
	inline QString StringTToQtString(std::string const &s)	{ return StringToQtString(s); }

	#endif // QSTRING_H

	typedef char char_t;
	typedef std::string string_t;
	typedef std::stringstream stringstream_t;
	//typedef std::ofstream ofstream_t;
	//typedef std::ifstream ifstream_t;
	typedef std::ostream ostream_t;

	#define cout_t cout
	#define cerr_t cerr
	#define cin_t cin

	#ifndef _T
	#define _T(x) x
	#endif // _T

	#define WIDEN(x) _T(x)
	#define WFILE __FILE__

	#define snprintf_t	snprintf
	#define sprintf_t	sprintf
	#define strcpy_t	strcpy

	#endif

	} // namespace string
} // namespace supportlib

#endif /* SUPPORTTYPES_H_ */

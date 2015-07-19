/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef HELPER_STRING_H
#define HELPER_STRING_H

#include <vector>
#include "support/support_dll_api.h"
#include "support/unicode/unicode_types.h"

namespace supportlib
{
	namespace string
	{
		SUPPORT_DLL_EXPORT std::vector<supportlib::string::string_t> split(supportlib::string::string_t const &s, supportlib::string::char_t delim);
		SUPPORT_DLL_EXPORT std::vector<supportlib::string::string_t> &split(supportlib::string::string_t const &s, supportlib::string::char_t delim, std::vector<supportlib::string::string_t> &elems);

		/**
		 * maskString replaces the characters '\', '\r', '\t' and '\n'
		 * with the literal characters, so that a string results that can be stored as a single line.
		 */
		SUPPORT_DLL_EXPORT supportlib::string::string_t maskString(supportlib::string::string_t const &oInput);

		/**
		 * unmaskString converts a string which was masked with unmaskString() back to it's original
		 * form.
		 */
		SUPPORT_DLL_EXPORT supportlib::string::string_t unmaskString(supportlib::string::string_t const &oInput);
	}
}

#endif // HELPER_STRING_H

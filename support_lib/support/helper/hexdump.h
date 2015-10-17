/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef HELPER_HEXDUMP_H
#define HELPER_HEXDUMP_H

#include "support/support_dll_api.h"
#include "support/unicode/unicode_types.h"

SUPPORT_DLL_EXPORT spt::string::string_t toHexArray(std::vector<char> const &oBuffer);

#endif // HELPER_HEXDUMP_H

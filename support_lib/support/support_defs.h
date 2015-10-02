/*******************************************************************************
 *
 * Support Library (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __SUPPORT_DEFS_H__
#define __SUPPORT_DEFS_H__

#define UNUSED(x) (void)(x)

// Makro to generate a string out of a makroparameter
// #define __DEFINE_TO_STRING(Test)  -> "Test"
#define __STR2__(x) #x
#define __DEFINE_TO_STRING(x) __STR2__(x)

#endif // __SUPPORT_DEFS_H__

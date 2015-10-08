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

/**
 * Macro for generating default member variables with getters and setters.
 *
 * BASE_MEMBER_SCOPED allows for each component to set the visibillity
 * seperately.
 * BASE_MEMBER is a shortcut where the variable is always private and the
 * getters and setters are at the defined visibillity.
 *
 */
#define BASE_MEMBER_SCOPED(getter_scope, setter_scope, member_scope, name, type) \
	getter_scope: \
	inline type get##name(void) const { return m##name;} \
	setter_scope: \
	inline void set##name(type name) { m##name = name; } \
	member_scope:\
	type m##name

#define BASE_MEMBER(scope, name, type) BASE_MEMBER_SCOPED(scope, scope, private, name, type)

/**
 * sizeof struct/class member.
 * Example: sizeof_member(mystruct, membername)
 */
#define sizeof_member(type, member) sizeof(((type *)0)->member)
#define member_offset(type, member) (size_t)&(((type *)0)->member)

#ifdef __cplusplus
#define C_TYPE extern "C"
#else
#define C_TYPE
#endif // __cplusplus

#ifndef __cplusplus
typedef int bool;
#define true 1
#define false 0
#endif

#ifndef __int8
#define __int8		char
#endif

#ifndef __int16
#define __int16		short
#endif

#ifndef __int32
#define __int32		int
#endif

#ifndef __int64
#define __int64		long long
#endif

typedef signed __int8			byte_t;
typedef unsigned __int8			ubyte_t;
typedef	signed __int8			int8_t;
typedef	unsigned __int8			uint8_t;

typedef	signed __int16			word_t;
typedef	unsigned __int16		uword_t;
typedef	signed __int16			int16_t;
typedef	unsigned __int16		uint16_t;

typedef	signed __int32			int_t;
typedef	unsigned __int32		uint_t;
typedef	signed __int32			int32_t;
typedef	unsigned __int32		uint32_t;

#ifdef _WIN64
typedef signed long long		ssize_t;		// size_t is already used by the standard, so we have to use this one
#else
typedef signed int				ssize_t;		// size_t is already used by the standard, so we have to use this one
#endif	// _WIN64

typedef	signed __int64			int64_t;
typedef	unsigned __int64		uint64_t;

#endif // __SUPPORT_DEFS_H__

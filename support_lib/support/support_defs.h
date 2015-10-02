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
	type m##name;

#define BASE_MEMBER(scope, name, type) BASE_MEMBER_SCOPED(scope, scope, private, name, type)

/**
 * sizeof struct/class member.
 * Example: sizeof_member(mystruct, membername)
 */
#define sizeof_member(type, member) sizeof(((type *)0)->member)


#endif // __SUPPORT_DEFS_H__

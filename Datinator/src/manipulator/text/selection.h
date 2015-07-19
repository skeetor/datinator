/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef TEXT_SELECTION_H_INCLUDED
#define TEXT_SELECTION_H_INCLUDED

typedef enum
{
	SEL_Text,				// Fixed Text
	SEL_Limit,				// Maximum/minimum length
	SEL_ReplacePattern,		// Replace text by using fixed text or a regex
	SEL_SelectPattern,		// Take text by using a regex
	SEL_SelectPosition,		// Take text by position
	SEL_ToUpper,			// Convert to lower case
	SEL_ToLower,			// Convert to lower case
	SEL_Multiline,			// Convert multiline string to singleline or vice versa (this masks certain characters like \n, \r, etc.)
	SEL_MAX
} TextManipulatorAction;

#endif // TEXT_MANIPULATOR_H_INCLUDED

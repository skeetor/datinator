/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef LOGIN_EVENT_H_INCLUDED
#define LOGIN_EVENT_H_INCLUDED

#include "support_qt/support_qt_dll_api.h"

#include "support/patterns/event.h"
#include "support/db/database_login.h"

class DatabaseLoginPanel;

class SUPPORT_QT_DLL_EXPORT LoginEvent : public Event<DatabaseLogin, DatabaseLoginPanel *>
{
public:
	LoginEvent(void);
	LoginEvent(LoginEvent const &oEvent);
	LoginEvent(DatabaseLoginPanel * const &oSource, DatabaseLogin const &oEvent, bool bActivated);

	virtual ~LoginEvent(void);

	virtual LoginEvent &copy(LoginEvent const &oSource);
	virtual LoginEvent &operator=(LoginEvent const &oSource);

	/**
	 * If true, then the login has been selected.
	 * If false, the login data has been deselected or removed.
	 */
	inline bool isActivated(void) const { return mActivated; }
	inline void setActivated(bool bActivated = true) { mActivated = bActivated; }

private:
	bool mActivated;
};

#endif // LOGIN_EVENT_H_INCLUDED

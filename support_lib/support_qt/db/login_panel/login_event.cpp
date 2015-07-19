/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "support_qt/db/login_panel/login_event.h"

LoginEvent::LoginEvent(void)
{
	mActivated = true;
}

LoginEvent::~LoginEvent(void)
{
}

LoginEvent::LoginEvent(LoginEvent const &oEvent)
: Event(oEvent)
{
	copy(oEvent);
}

LoginEvent::LoginEvent(DatabaseLoginPanel * const &oSource, DatabaseLogin const &oEvent, bool bActivated)
: Event(oSource, oEvent)
{
	setActivated(bActivated);
}

LoginEvent &LoginEvent::operator=(LoginEvent const &oSource)
{
	return copy(oSource);
}

LoginEvent &LoginEvent::copy(LoginEvent const &oSource)
{
    Event::copy(oSource);
    setActivated(oSource.isActivated());

	return *this;
}

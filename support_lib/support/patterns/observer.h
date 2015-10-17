/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

// The variadic template mechanism requires at least a C++11 compiler.

#ifndef OBSERVER_TEMPLATE_INCLUDED_H
#define OBSERVER_TEMPLATE_INCLUDED_H

#include "support/support_defs.h"
#include <vector>

#ifndef ListenerList
#define ListenerList		std::vector
#endif

#ifndef ListenerList
#error No List class defined for the listener. Please either define USE_QT_LISTENER or USE_STD_LISTENER or include the appropriate header file before observer.h
#endif // ListenerList

template <typename... T> class Dispatcher;

template <typename... T>
class Listener
{
public:
	Listener(void)
	{
	}

	virtual ~Listener(void)
	{
	}

	virtual void handleNotification(Dispatcher<T...> *oSource, T... /* oEvent */)
	{
		// Default implementation does nothing
		// which can be used as a null listener where
		// a listener is expected but doesn't have
		// a meaningful implementation.
		UNUSED(oSource);
	}

	/**
	 * The invalidateDispatcher() call is sent when the dispatcher
	 * should no longer be accessed anymore. After this call, the listener
	 * will no longer receive any notifications and the dispatcher is
	 * destroyed, so the listener should not unregister with
	 * removeListener().
	 */
	virtual void invalidateDispatcher(Dispatcher<T...> const *oDispatcher)
	{
		UNUSED(oDispatcher);
	}
};

template <typename... T>
class Dispatcher
{
public:
	Dispatcher(void)
	{
		mAllowDuplicates = false;
	}

	virtual ~Dispatcher(void)
	{
		invalidate();
	}

	void allowDuplicates(bool bAllowDuplicates = true)
	{
		mAllowDuplicates = bAllowDuplicates;
	}

	/**
	 * After the invalidate() message is sent to the listeners,
	 * they will no longer receive any notifications and they should
	 * no longer access the dispatcher pointer as the object became invalid.
	 * When this call is sent, the listener also shouldn't
	 * unregister via removeListener().
	 */
	virtual void invalidate(void)
	{
		for(Listener<T...> * &listener : mListeners)
			listener->invalidateDispatcher(this);
	}

	virtual void notify(T... oEvent)
	{
		for(Listener<T...> * &listener : mListeners)
			listener->handleNotification(this, oEvent...);
	}

	/**
	 * Adds a listener to the dispatcher. A listener
	 * can attach itself multiple times, in which case
	 * it will receive as many notifications as it
	 * is registered. When the listener is removed
	 * it will remove all instances with a single call
	 * so there is no need to balance the addListener()
	 * with removeListener() calls.
	 */
	virtual void addListener(Listener<T...> *oListener)
	{
		if(!mAllowDuplicates)
		{
			if(listenerIndex(oListener) != -1)
				return;
		}

		mListeners.push_back(oListener);
	}

	virtual void removeListener(Listener<T...> *oListener)
	{
		// The listener may have registered multiple times
		// so we must remove all instances.
		ssize_t i;
		while((i = listenerIndex(oListener)) != -1)
			mListeners.erase(mListeners.begin() + i);
	}

protected:
	ListenerList<Listener<T...> *> &getListeners(void) const
	{
		return mListeners;
	}

	virtual ssize_t listenerIndex(Listener<T...> const *oListener) const
	{
		ssize_t i = -1;
		for(Listener<T...> * const &listener : mListeners)
		{
			i++;
			if(listener == oListener)
				return i;
		}

		return -1;
	}

 private:
	ListenerList<Listener<T...> *> mListeners;
	bool mAllowDuplicates;
};

#endif // OBSERVER_TEMPLATE_INCLUDED_H

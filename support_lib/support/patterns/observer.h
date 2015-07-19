/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

// The variadic template mechanism requires at least a C++11 compiler.

/**
 * The listener can use a QList oder a std::vector for maintaining it's list.
 * To use one or the other, simply include the appropriate header before
 * including the listener file. To force a particular one you must define
 * which one it should be. If both lists are included (QList and vector)
 * QList will be used. If none are provided, std::vector is assumed.
 *
 * USE_QT_LISTENER
 * USE_STD_LISTENER
 */
#ifndef OBSERVER_TEMPLATE_INCLUDED_H
#define OBSERVER_TEMPLATE_INCLUDED_H

#include "support/support_defs.h"

#if !defined(USE_QT_LISTENER)
#   if !defined(USE_STD_LISTENER)
#      if defined(QLIST_H)
#         define USE_QT_LISTENER
#       endif // QLIST_H
#   else
#       define USE_STD_LISTENER      // default
#   endif // USE_STD_LISTENER
#endif // USE_QT_LISTENER

#if !defined(USE_STD_LISTENER)
#   if defined(_GLIBCXX_VECTOR)
#      define USE_QT_LISTENER
#    endif // _GLIBCXX_VECTOR
#endif // USE_STD_LISTENER

#if defined(USE_QT_LISTENER)
#define ListenerList		QList
#endif // QLIST_H

#if defined(USE_STD_LISTENER)
#define ListenerList		std::vector
#endif // _GLIBCXX_VECTOR

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
		int i;
		while((i = listenerIndex(oListener)) != -1)
			mListeners.erase(mListeners.begin() + i);
	}

protected:
	ListenerList<Listener<T...> *> &getListeners(void) const
	{
		return mListeners;
	}

	virtual int listenerIndex(Listener<T...> const *oListener) const
	{
		int i = -1;
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

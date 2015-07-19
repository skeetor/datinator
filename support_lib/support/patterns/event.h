/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef EVENT_INTERFACE_INCLUDED_H
#define EVENT_INTERFACE_INCLUDED_H

/**
 * Sends an even of type T from the source object S
 */
template <typename T, typename S>
class Event
{
public:
	Event(void) {};

	Event(S const &oSource, T const &oEvent)
	{
		setSource(oSource);
		setEvent(oEvent);
	}

	Event(Event const &oSource)
	{
		copy(oSource);
	}

	virtual Event &copy(Event const &oSource)
	{
		setSource(oSource.getSource());
		setEvent(oSource.getEvent());
		return *this;
	}

	virtual Event &operator=(Event const &oSource)
	{
		return copy(oSource);
	}

	virtual ~Event(void) {};

	virtual S getSource(void) const
	{
		return mSource;
	}

	virtual T getEvent(void) const
	{
		return mEvent;
	}

protected:
	virtual void setSource(S const &oSource)
	{
		mSource = oSource;
	}

	virtual void setEvent(T const &oEvent)
	{
		mEvent = oEvent;
	}

private:
	S mSource;
	T mEvent;
};

#endif // EVENT_INTERFACE_INCLUDED_H

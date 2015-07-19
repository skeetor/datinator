/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef QT_SIGNAL_BLOCKER_HELPER_H
#define QT_SIGNAL_BLOCKER_HELPER_H

#include <QtCore/QObject>

#include "support_qt/support_qt_dll_api.h"

class SignalBlocker
{
public:
    SignalBlocker(QObject *oObject)
    {
    	mState = false;
		mObject = oObject;
		if(mObject)
			mState = mObject->blockSignals(true);
	}

    virtual ~SignalBlocker()
    {
    	if(mObject)
			mObject->blockSignals(mState);
    }

private:
    QObject *mObject;
    bool mState;
};

#endif // QT_SIGNAL_BLOCKER_HELPER_H

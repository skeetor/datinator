/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "manipulator/manipulator_panel_qt.moc"

ManipulatorPanel::ManipulatorPanel(Manipulator *oOwner, QWidget *oParent)
: QFrame(oParent)
{
	mMode = Manipulator::PREPEND;
	mOwner = oOwner;
	mPrependBtn = NULL;
	mAppendBtn = NULL;
	mReplaceBtn = NULL;
}

ManipulatorPanel::~ManipulatorPanel(void)
{
}

void ManipulatorPanel::setButtons(QRadioButton *oPrependBtn, QRadioButton *oAppendBtn, QRadioButton *oReplaceBtn)
{
	mPrependBtn = oPrependBtn;
	mAppendBtn = oAppendBtn;
	mReplaceBtn = oReplaceBtn;
}

void ManipulatorPanel::notifyOwner()
{
	mOwner->notifyConfigChangedListeners(mOwner);
}

Manipulator::Mode ManipulatorPanel::getMode(void) const
{
	return mMode;
}

/*
void ManipulatorPanel::setMode(Manipulator::Mode nMode)
{
	mMode = nMode;
}
*/

void ManipulatorPanel::setMode(Manipulator::Mode nMode)
{
	mMode = nMode;
	switch(nMode)
	{
		case Manipulator::Mode::PREPEND:
		{
			if(mPrependBtn)
				mPrependBtn->setChecked(true);

			onPrepend();
		}
		break;

		case Manipulator::Mode::APPEND:
		{
			if(mAppendBtn)
				mAppendBtn->setChecked(true);
			onAppend();
		}
		break;

		default:
		{
			if(mReplaceBtn)
				mReplaceBtn->setChecked(true);
			onReplace();
		}
	}
}

void ManipulatorPanel::onAppend(void)
{
	mMode = Manipulator::Mode::APPEND;
	notifyOwner();
}

void ManipulatorPanel::onPrepend(void)
{
	mMode = Manipulator::Mode::PREPEND;
	notifyOwner();
}

void ManipulatorPanel::onReplace(void)
{
	mMode = Manipulator::Mode::REPLACE;
	notifyOwner();
}

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "manipulator/autonumber/autonumber_panel_gui.moc"
#include "manipulator/autonumber/autonumber.h"

AutoNumberPanel::AutoNumberPanel(AutoNumberManipulator *oOwner, QWidget *oParent)
: ManipulatorPanel(oOwner, oParent)
{
	mGUI = new Ui::AutoNumberGUI();
	mGUI->setupUi(this);
	setButtons(mGUI->mPrependBtn, mGUI->mAppendBtn, mGUI->mReplaceBtn);
	mLeadingZeroes = false;
}

AutoNumberPanel::~AutoNumberPanel(void)
{
}

void AutoNumberPanel::onWidthChanged(int nWidth)
{
	if(nWidth > 0)
	{
		mGUI->mLeadingZeroesBox->setChecked(true);
		mGUI->mLeadingZeroesBox->setEnabled(true);
		mLeadingZeroes = true;
	}
	else
	{
		mGUI->mLeadingZeroesBox->setChecked(false);
		mGUI->mLeadingZeroesBox->setEnabled(false);
		mLeadingZeroes = false;
	}

	notifyOwner();
}

void AutoNumberPanel::onValueChanged(int nValue)
{
	UNUSED(nValue);

	notifyOwner();
}

void AutoNumberPanel::onIncrementChanged(int nIncrement)
{
	UNUSED(nIncrement);

	notifyOwner();
}

void AutoNumberPanel::onLeadingZeroes(bool bLeadingZeroes)
{
	mLeadingZeroes = bLeadingZeroes;
	notifyOwner();
}

int AutoNumberPanel::getValue(void) const
{
	return mGUI->mValueBox->value();
}

void AutoNumberPanel::setValue(int nValue)
{
	mGUI->mValueBox->setValue(nValue);
}

int AutoNumberPanel::getWidth(void) const
{
	return 	mGUI->mWidthBox->value();
}

void AutoNumberPanel::setWidth(int nWidth)
{
	mGUI->mWidthBox->setValue(nWidth);
}

int AutoNumberPanel::getIncrement(void) const
{
	return mGUI->mIncrementBox->value();
}

void AutoNumberPanel::setIncrement(int nIncrement)
{
	mGUI->mIncrementBox->setValue(nIncrement);
}

bool AutoNumberPanel::getLeadingZeroes(void) const
{
	return mLeadingZeroes;
}

void AutoNumberPanel::setLeadingZeroes(bool bLeadingZeroes)
{
	mLeadingZeroes = bLeadingZeroes;
	mGUI->mLeadingZeroesBox->setChecked(bLeadingZeroes);
}

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

void AutoNumberPanel::onWidthChanged(size_t nWidth)
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

void AutoNumberPanel::onValueChanged(size_t nValue)
{
	UNUSED(nValue);

	notifyOwner();
}

void AutoNumberPanel::onIncrementChanged(size_t nIncrement)
{
	UNUSED(nIncrement);

	notifyOwner();
}

void AutoNumberPanel::onLeadingZeroes(bool bLeadingZeroes)
{
	mLeadingZeroes = bLeadingZeroes;
	notifyOwner();
}

size_t AutoNumberPanel::getValue(void) const
{
	return mGUI->mValueBox->value();
}

void AutoNumberPanel::setValue(size_t nValue)
{
	mGUI->mValueBox->setValue(nValue);
}

size_t AutoNumberPanel::getWidth(void) const
{
	return 	mGUI->mWidthBox->value();
}

void AutoNumberPanel::setWidth(size_t nWidth)
{
	mGUI->mWidthBox->setValue(nWidth);
}

size_t AutoNumberPanel::getIncrement(void) const
{
	return mGUI->mIncrementBox->value();
}

void AutoNumberPanel::setIncrement(size_t nIncrement)
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

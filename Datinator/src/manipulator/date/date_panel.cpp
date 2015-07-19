/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "manipulator/date/date_panel_gui.moc"
#include "manipulator/date/date.h"

DateManipulatorPanel::DateManipulatorPanel(DateManipulator *oOwner, QWidget *oParent)
: ManipulatorPanel(oOwner, oParent)
{
	mGUI = new Ui::DateManipulatorGUI();
	mGUI->setupUi(this);
	setButtons(mGUI->mPrependBtn, mGUI->mAppendBtn, mGUI->mReplaceBtn);

	mGUI->mSysdateStartBox->setToolTip("If checked, sysdate is taken at start of processing, otherwise at time of insert.");
	initCombobox(mGUI->mInputBox);
	initCombobox(mGUI->mOutputBox);
	mGUI->mOutputBox->removeItem(DateManipulator::DateFormat::DT_SYSDATE);
}

DateManipulatorPanel::~DateManipulatorPanel(void)
{
}

void DateManipulatorPanel::initCombobox(QComboBox *oBox)
{
	QList<QString> const &fmts = DateManipulator::getFormatTexts();
	for(int i = 0; i < DateManipulator::DateFormat::DT_MAX; i++)
		oBox->addItem(fmts[i], i);
}

bool DateManipulatorPanel::isSysdateStart(void) const
{
	if(mGUI->mSysdateStartBox->isChecked())
		return true;

	return false;
}

void DateManipulatorPanel::setSysdateStart(bool bSysdateStart)
{
	if(bSysdateStart)
		mGUI->mSysdateStartBox->setChecked(true);
	else
		mGUI->mSysdateStartBox->setChecked(false);
	notifyOwner();
}

bool DateManipulatorPanel::isSysdate(void) const
{
	QVariant v = mGUI->mInputBox->currentData();
	DateManipulator::DateFormat fmt = static_cast<DateManipulator::DateFormat>(v.value<int>());
	if(fmt == DateManipulator::DateFormat::DT_SYSDATE)
		return true;

	return false;
}

void DateManipulatorPanel::setSysdate(bool bSysdate)
{
	if(bSysdate)
	{
		mGUI->mInputBox->setCurrentIndex(DateManipulator::DateFormat::DT_SYSDATE);
		mGUI->mInputFormatTxt->setEnabled(false);
	}
	mGUI->mSysdateStartBox->setEnabled(bSysdate);

	notifyOwner();
}

void DateManipulatorPanel::onSysdateStart(bool bSysdateStart)
{
	UNUSED(bSysdateStart);

	notifyOwner();
}

QString DateManipulatorPanel::getFormat(QComboBox *oBox, QLineEdit *oCustomFormat) const
{
	QVariant v = oBox->currentData();
	int fmt = v.value<int>();
	QList<QString> const &fmts = DateManipulator::getFormatTexts();
	if(fmt == DateManipulator::DateFormat::DT_CUSTOM)
		return oCustomFormat->text();
	else if(fmt < fmts.size())
		return fmts[fmt];

	return "";
}

void DateManipulatorPanel::setFormat(QString const &oString, QComboBox *oBox, QLineEdit *oCustomFormat)
{
	oCustomFormat->setText("");
	if(oString.length() == 0)
	{
		oBox->setCurrentIndex(DateManipulator::DateFormat::DT_SYSDATE);
		return;
	}

	QList<QString> const &fmts = DateManipulator::getFormats();
	int r = oBox->count();
	for(int i = 0; i < r; i++)
	{
		if(fmts[i] == oString)
		{
			oBox->setCurrentIndex(i);
			return;
		}
	}

	oBox->setCurrentIndex(DateManipulator::DateFormat::DT_CUSTOM);
	oCustomFormat->setText(oString);
}

QString DateManipulatorPanel::getInputFormat(void) const
{
	return getFormat(mGUI->mInputBox, mGUI->mInputFormatTxt);
}

void DateManipulatorPanel::setInputFormat(QString const &oFormat)
{
	setFormat(oFormat, mGUI->mInputBox, mGUI->mInputFormatTxt);
}

QString DateManipulatorPanel::getOutputFormat(void) const
{
	return getFormat(mGUI->mOutputBox, mGUI->mOutputFormatTxt);
}

void DateManipulatorPanel::setOutputFormat(QString const &oFormat)
{
	setFormat(oFormat, mGUI->mOutputBox, mGUI->mOutputFormatTxt);
}

void DateManipulatorPanel::onTextChanged(QString oText)
{
	UNUSED(oText);

	notifyOwner();
}

void DateManipulatorPanel::formatChanged(QComboBox *oBox, QLineEdit *oCustomFormat, bool bInput)
{
	DateManipulator::DateFormat fmt = static_cast<DateManipulator::DateFormat>(oBox->currentIndex());
	switch(fmt)
	{
		case DateManipulator::DateFormat::DT_CUSTOM:
		{
			oCustomFormat->setEnabled(true);
			if(bInput)
				mGUI->mSysdateStartBox->setEnabled(false);
		}
		break;

		case DateManipulator::DateFormat::DT_SYSDATE:
		{
			oCustomFormat->setEnabled(false);
			if(bInput)
				mGUI->mSysdateStartBox->setEnabled(true);
		}
		break;

		default:
		{
			oCustomFormat->setEnabled(false);
			if(bInput)
				mGUI->mSysdateStartBox->setEnabled(false);
		}
		break;
	}
}

void DateManipulatorPanel::onInputFormat(int nSelection)
{
	UNUSED(nSelection);

	formatChanged(mGUI->mInputBox, mGUI->mInputFormatTxt, true);
	notifyOwner();
}

void DateManipulatorPanel::onOutputFormat(int nSelection)
{
	UNUSED(nSelection);

	formatChanged(mGUI->mOutputBox, mGUI->mOutputFormatTxt, false);
	notifyOwner();
}

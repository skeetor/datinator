/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QSettings>

#include "manipulator/autonumber/autonumber.h"

AutoNumberManipulator::AutoNumberManipulator(void)
{
	mPanel = NULL;
	init();
}

AutoNumberManipulator::AutoNumberManipulator(AutoNumberManipulator const &oSource)
{
	mPanel = NULL;
	init();
	copy(oSource);
}

AutoNumberManipulator::~AutoNumberManipulator(void)
{
}

int AutoNumberManipulator::getValue(void) const
{
	return mValue;
}

void AutoNumberManipulator::setValue(int nValue)
{
	mValue = nValue;
}

int AutoNumberManipulator::getWidth(void) const
{
	return mWidth;
}

void AutoNumberManipulator::setWidth(int nWidth)
{
	mWidth = nWidth;
}

int AutoNumberManipulator::getIncrement(void) const
{
	return mIncrement;
}

void AutoNumberManipulator::setIncrement(int nIncrement)
{
	mIncrement = nIncrement;
}

bool AutoNumberManipulator::getLeadingZeroes(void) const
{
	return mLeadingZeroes;
}

void AutoNumberManipulator::setLeadingZeroes(bool bLeadingZeroes)
{
	mLeadingZeroes = bLeadingZeroes;
}

bool AutoNumberManipulator::isConfigured(void)
{
	readValues();
	return true;
}

QString AutoNumberManipulator::getId(void)
{
	return "53200A70-0360-11E4-9191-0800200C9A66";
}

void AutoNumberManipulator::init(void)
{
	setMode(Manipulator::Mode::REPLACE);
	setValue(1);
	setIncrement(1);
	setWidth(0);
	setLeadingZeroes(true);
	setValues();
}

void AutoNumberManipulator::reset(void)
{
	Manipulator::reset();
	readValues();
}

AutoNumberManipulator &AutoNumberManipulator::operator=(AutoNumberManipulator const &oSource)
{
	init();
	copy(oSource);
	return *this;
}

void AutoNumberManipulator::copy(AutoNumberManipulator const &oSource)
{
	init();

	Manipulator::copy(oSource);
	setValue(oSource.getValue());
	setIncrement(oSource.getIncrement());
	setWidth(oSource.getWidth());
	setLeadingZeroes(oSource.getLeadingZeroes());
}

QString AutoNumberManipulator::getName(void)
{
	return "Automatic numbering";
}

QString AutoNumberManipulator::getDescription(void)
{
	return "Creates an incremental number";
}

IManipulator *AutoNumberManipulator::createInstance(void)
{
	return new AutoNumberManipulator();
}

IManipulator *AutoNumberManipulator::duplicate(void)
{
	return new AutoNumberManipulator(*this);
}

void AutoNumberManipulator::prepare(void)
{
	Manipulator::prepare();
	readValues();
}

void AutoNumberManipulator::readValues(void)
{
	if(!mPanel)
		return;

	setMode(mPanel->getMode());
	setValue(mPanel->getValue());
	setIncrement(mPanel->getIncrement());
	setWidth(mPanel->getWidth());
	setLeadingZeroes(mPanel->getLeadingZeroes());
}

void AutoNumberManipulator::setValues(void)
{
	if(!mPanel)
		return;

	mPanel->setMode(getMode());
	mPanel->setValue(getValue());
	mPanel->setIncrement(getIncrement());
	mPanel->setWidth(getWidth());
	mPanel->setLeadingZeroes(getLeadingZeroes());
}

QWidget *AutoNumberManipulator::getConfigurationPanel(QWidget *oParent)
{
	if(mPanel == NULL)
		mPanel = new AutoNumberPanel(this, oParent);

	setValues();

	return mPanel;
}

QString AutoNumberManipulator::createString(void) const
{
	QString result;
	QTextStream(&result) << mValue;

	int w = getWidth();
	if(w > 0)
	{
		QString c = " ";
		if(getLeadingZeroes())
			c = "0";

		while(result.length() < w)
			result = c+result;
	}

	return result;
}

/**
 * If the input string has some value, it is only used
 * if no Text is specified by the user. If this is the case
 */
QString *AutoNumberManipulator::format(QString *oValue, bool bPreview)
{
	QString result = createString();
	if(!bPreview)
		setValue(getValue() + mIncrement);

	return applyMode(oValue, result);
}

bool AutoNumberManipulator::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!Manipulator::loadProfile(oProfile, oKey))
		return false;

	setValue(oProfile.value(oKey+"_value", "1").toInt());
	setIncrement(oProfile.value(oKey+"_increment", "1").toInt());
	setWidth(oProfile.value(oKey+"_width", "1").toInt());
	setLeadingZeroes(oProfile.value(oKey+"_leading_zero", "").toBool());

	getConfigurationPanel(NULL);
	setValues();

	return true;
}

void AutoNumberManipulator::saveProfile(QSettings &oProfile, QString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);

	readValues();
	oProfile.setValue(oKey+"_value", QString::number(getValue()));
	oProfile.setValue(oKey+"_increment", QString::number(getIncrement()));
	oProfile.setValue(oKey+"_width", QString::number(getWidth()));
	oProfile.setValue(oKey+"_leading_zero", QString::number(getLeadingZeroes()));
}

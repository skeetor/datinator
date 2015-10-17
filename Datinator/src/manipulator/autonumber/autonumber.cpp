/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QTextStream>
#include <QtCore/QSettings>
#include <QtCore/QString>

#include "manipulator/autonumber/autonumber.h"
#include "support/helper/string.h"

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

size_t AutoNumberManipulator::getValue(void) const
{
	return mValue;
}

void AutoNumberManipulator::setValue(size_t nValue)
{
	mValue = nValue;
}

size_t AutoNumberManipulator::getWidth(void) const
{
	return mWidth;
}

void AutoNumberManipulator::setWidth(size_t nWidth)
{
	mWidth = nWidth;
}

size_t AutoNumberManipulator::getIncrement(void) const
{
	return mIncrement;
}

void AutoNumberManipulator::setIncrement(size_t nIncrement)
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

StdString AutoNumberManipulator::getId(void)
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

StdString AutoNumberManipulator::getName(void)
{
	return "Automatic numbering";
}

StdString AutoNumberManipulator::getDescription(void)
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

StdString AutoNumberManipulator::createString(void) const
{
	StdString result = spt::string::toString(mValue);

	size_t w = getWidth();
	if(w > 0)
	{
		StdString c = " ";
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
StdString *AutoNumberManipulator::format(StdString *oValue, bool bPreview)
{
	StdString result = createString();
	if(!bPreview)
		setValue(getValue() + mIncrement);

	return applyMode(oValue, result);
}

bool AutoNumberManipulator::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	auto key = spt::string::toQt(oKey);

	if(!Manipulator::loadProfile(oProfile, oKey))
		return false;

	setValue(oProfile.value(key+"_value", "1").toInt());
	setIncrement(oProfile.value(key+"_increment", "1").toInt());
	setWidth(oProfile.value(key+"_width", "1").toInt());
	setLeadingZeroes(oProfile.value(key+"_leading_zero", "").toBool());

	getConfigurationPanel(NULL);
	setValues();

	return true;
}

void AutoNumberManipulator::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);

	auto key = spt::string::toQt(oKey);

	readValues();
	oProfile.setValue(key+"_value", QString::number(getValue()));
	oProfile.setValue(key+"_increment", QString::number(getIncrement()));
	oProfile.setValue(key+"_width", QString::number(getWidth()));
	oProfile.setValue(key+"_leading_zero", QString::number(getLeadingZeroes()));
}

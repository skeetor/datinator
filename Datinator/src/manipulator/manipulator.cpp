/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/


#include <QtCore/QSettings>

#include "manipulator/manipulator.h"
#include "support/helper/string.h"

Manipulator::Manipulator(void)
{
	mMode = Manipulator::Mode::REPLACE;
	mValid = true;
}

Manipulator::~Manipulator(void)
{
}

void Manipulator::copy(Manipulator const &oSource)
{
	setMode(oSource.getMode());
	setTestValue(oSource.getTestValue());
	setSourceColumns(oSource.getSourceColumns());
}

void Manipulator::addConfigChangeListener(Listener<IManipulator *> *oListener)
{
	Dispatcher<IManipulator *>::addListener(oListener);
}

void Manipulator::removeConfigChangeListener(Listener<IManipulator *> *oListener)
{
	Dispatcher<IManipulator *>::removeListener(oListener);
}

void Manipulator::notifyConfigChangedListeners(IManipulator *oManipulator)
{
	notify(oManipulator);
}

void Manipulator::prepare(void)
{
	setValid();
	setError("");
}

void Manipulator::reset(void)
{
	setError("");
	setValid();
	setMode(Manipulator::Mode::REPLACE);
}

StdString Manipulator::lastError(void)
{
	return mErrorText;
}

void Manipulator::setError(StdString const &oText)
{
	mErrorText = oText;
}

bool Manipulator::isValid(void) const
{
	return mValid;
}

void Manipulator::setValid(bool bValid)
{
	mValid = bValid;
}

Manipulator::Mode Manipulator::getMode(void) const
{
	return mMode;
}

void Manipulator::setMode(Manipulator::Mode nMode)
{
	mMode = nMode;
}

StdString *Manipulator::applyMode(StdString *oValue, StdString const &oManipulatorString)
{
	StdString *s = oValue;

	if(s == NULL)
		s = new StdString();

	switch(getMode())
	{
		case Manipulator::Mode::PREPEND:
			*s = oManipulatorString+*s;
		break;

		case Manipulator::Mode::APPEND:
			*s += oManipulatorString;
		break;

		default:
			*s = oManipulatorString;
		break;
	}

	return s;
}

bool Manipulator::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	auto s = oProfile.value(spt::string::toQt(oKey+"_mode"), "R").toString();

	Manipulator::Mode m;
	if(s == "P")
		m = Manipulator::Mode::PREPEND;
	else if(s == "A")
		m = Manipulator::Mode::APPEND;
	else if(s == "R")
		m = Manipulator::Mode::REPLACE;
	else
		return false;

	setMode(m);
	setTestValue(spt::string::fromQt(oProfile.value(spt::string::toQt(oKey+"_test_value")).toString()));

	return true;
}

void Manipulator::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	StdString s;
	auto key = spt::string::toQt(oKey);

	switch(getMode())
	{
		case Manipulator::Mode::PREPEND:
			s = "P";
		break;

		case Manipulator::Mode::APPEND:
			s = "A";
		break;

		default:
			s = "R";
		break;
	}
	oProfile.setValue(key+"_mode", spt::string::toQt(s));
	oProfile.setValue(key+"_test_value", spt::string::toQt(getTestValue()));
}

void Manipulator::readValues(void)
{
}

void Manipulator::setValues(void)
{
}

void Manipulator::setSourceColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	mColumns = oColumns;
}

std::vector<DatabaseColumn *> Manipulator::getSourceColumns(void) const
{
	return mColumns;
}

StdString Manipulator::toString(void)
{
	readValues();
	StdString *v = new StdString(getTestValue());
	v = format(v, true);
	StdString s;
	if(v)
	{
		s = *v;
		delete v;
	}

	return "["+s+"]";
}

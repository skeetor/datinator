/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QSettings>

#include "manipulator/manipulator.h"

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

QString Manipulator::lastError(void)
{
	return mErrorText;
}

void Manipulator::setError(QString const &oText)
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

QString *Manipulator::applyMode(QString *oValue, QString const &oManipulatorString)
{
	QString *s = oValue;

	if(s == NULL)
		s = new QString();

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

bool Manipulator::loadProfile(QSettings &oProfile, QString const &oKey)
{
	QString s = oProfile.value(oKey+"_mode", "R").toString();
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
	setTestValue(oProfile.value(oKey+"_test_value").toString());

	return true;
}

void Manipulator::saveProfile(QSettings &oProfile, QString const &oKey)
{
	QString s;
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
	oProfile.setValue(oKey+"_mode", s);
	oProfile.setValue(oKey+"_test_value", getTestValue());
}

void Manipulator::readValues(void)
{
}

void Manipulator::setValues(void)
{
}

void Manipulator::setSourceColumns(QList<DatabaseColumn *> const &oColumns)
{
	mColumns = oColumns;
}

QList<DatabaseColumn *> Manipulator::getSourceColumns(void) const
{
	return mColumns;
}

QString Manipulator::toString(void)
{
	readValues();
	QString *v = new QString(getTestValue());
	v = format(v, true);
	QString s;
	if(v)
	{
		s = *v;
		delete v;
	}

	return "["+s+"]";
}

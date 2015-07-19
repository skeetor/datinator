/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QString>

#include "manipulator/null/null.h"

NullManipulator::NullManipulator(void)
{
}

NullManipulator::~NullManipulator(void)
{
}

QString NullManipulator::getId(void)
{
	return "B7F61B90-FFC0-11E3-9191-0800200C9A66";
}

bool NullManipulator::isConfigured(void)
{
	return true;
}

QString NullManipulator::toString(void)
{
	return "[null]";
}

QString NullManipulator::getName(void)
{
	return "Null";
}

QString NullManipulator::getDescription(void)
{
	return "This manipulator will always return a null value.";
}

IManipulator *NullManipulator::createInstance(void)
{
	return new NullManipulator();
}

IManipulator *NullManipulator::duplicate(void)
{
	return new NullManipulator();
}

QWidget *NullManipulator::getConfigurationPanel(QWidget *oParent)
{
	UNUSED(oParent);

	return NULL;
}

QString *NullManipulator::format(QString *oValue, bool bPreview)
{
	UNUSED(bPreview);

	delete oValue;
	return NULL;
}

void NullManipulator::reset(void)
{
	Manipulator::reset();
}

void NullManipulator::prepare(void)
{
	Manipulator::prepare();
}

bool NullManipulator::loadProfile(QSettings &oProfile, QString const &oKey)
{
	return Manipulator::loadProfile(oProfile, oKey);
}

void NullManipulator::saveProfile(QSettings &oProfile, QString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);
}

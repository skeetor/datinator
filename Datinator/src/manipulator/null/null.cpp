/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "manipulator/null/null.h"

NullManipulator::NullManipulator(void)
{
}

NullManipulator::~NullManipulator(void)
{
}

StdString NullManipulator::getId(void)
{
	return "B7F61B90-FFC0-11E3-9191-0800200C9A66";
}

bool NullManipulator::isConfigured(void)
{
	return true;
}

StdString NullManipulator::toString(void)
{
	return "[null]";
}

StdString NullManipulator::getName(void)
{
	return "Null";
}

StdString NullManipulator::getDescription(void)
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

StdString *NullManipulator::format(StdString *oValue, bool bPreview)
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

bool NullManipulator::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	return Manipulator::loadProfile(oProfile, oKey);
}

void NullManipulator::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);
}

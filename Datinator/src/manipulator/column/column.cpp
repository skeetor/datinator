/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "support/unicode/unicode_types.h"
#include "support/db/dbcolumn.h"
#include "manipulator/column/column.h"
#include "support/helper/string.h"

ColumnManipulator::ColumnManipulator(void)
{
	mPanel = NULL;
	reset();
}

ColumnManipulator::ColumnManipulator(ColumnManipulator const &oSource)
{
	mPanel = NULL;
	reset();
	copy(oSource);
}

ColumnManipulator::~ColumnManipulator(void)
{
}

bool ColumnManipulator::isConfigured(void)
{
	readValues();

	return true;
}

StdString ColumnManipulator::getId(void)
{
	return "BBEF3EA0-FFC0-11E3-9191-0800200C9A67";
}

void ColumnManipulator::reset(void)
{
	Manipulator::reset();

	mColumnIndex = 0;

	setValues();
}

ColumnManipulator &ColumnManipulator::operator=(ColumnManipulator const &oSource)
{
	reset();
	copy(oSource);
	return *this;
}

void ColumnManipulator::copy(ColumnManipulator const &oSource)
{
	reset();
	Manipulator::copy(oSource);

	setColumn(oSource.getColumn());
	setValues();
}

StdString ColumnManipulator::getName(void)
{
	return "Column value";
}

StdString ColumnManipulator::getDescription(void)
{
	return "Returns the value from a different column";
}

IManipulator *ColumnManipulator::createInstance(void)
{
	return new ColumnManipulator();
}

IManipulator *ColumnManipulator::duplicate(void)
{
	return new ColumnManipulator(*this);
}

void ColumnManipulator::readValues(void)
{
	if(!mPanel)
		return;

	setMode(mPanel->getMode());
	setColumn(mPanel->getColumn());
}

void ColumnManipulator::setValues(void)
{
	if(!mPanel)
		return;

	mPanel->setMode(getMode());
	mPanel->setColumn(getColumn());
}

QWidget *ColumnManipulator::getConfigurationPanel(QWidget *oParent)
{
	if(mPanel == NULL)
		mPanel = new ColumnManipulatorPanel(this, oParent);

	setValues();

	return mPanel;
}

StdString *ColumnManipulator::format(StdString *oValue, bool bPreview)
{
	StdString s;
	StdString v;

	if(oValue)
		v = *oValue;

	ssize_t i = getColumn();
	std::vector<DatabaseColumn *> columns = getSourceColumns();
	if(i < (ssize_t)columns.size())
	{
		DatabaseColumn *col = columns[i];
		if(col)
		{
			if(bPreview)
				s = "("+col->getName()+")";
			else
				s = col->getValue();
		}
	}

	return applyMode(oValue, s);
}

bool ColumnManipulator::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	if(!Manipulator::loadProfile(oProfile, oKey))
		return false;

	getConfigurationPanel(NULL);
	setColumn(oProfile.value(spt::string::toQt(oKey+"_column_index")).toInt());

	setValues();
	return true;
}

void ColumnManipulator::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);

	readValues();
	oProfile.setValue(spt::string::toQt(oKey+"_column_index"), getColumn());
}

void ColumnManipulator::prepare(void)
{
	Manipulator::prepare();
	readValues();
}

void ColumnManipulator::setColumn(ssize_t nColumnIndex)
{
	mColumnIndex = nColumnIndex;
}

ssize_t ColumnManipulator::getColumn(void) const
{
	return mColumnIndex;
}

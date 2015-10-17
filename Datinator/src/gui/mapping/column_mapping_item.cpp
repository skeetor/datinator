/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <memory>

#include <QtCore/QSettings>

#include "support/unicode/unicode_types.h"
#include "support/db/dbcolumn.h"
#include "support/logging/logging.h"

#include "manipulator/imanipulator.h"
#include "column_mapping_item.h"
#include "plugin/gui/progress.h"

#include "support/helper/string.h"

Q_DECLARE_METATYPE(IManipulator *)

extern std::vector<IManipulator *> gReferences;

ColumnMappingItem::ColumnMappingItem(void)
{
	init();
}

ColumnMappingItem::ColumnMappingItem(ColumnMappingItem const &oSource)
{
	copy(oSource);
}

ColumnMappingItem::~ColumnMappingItem()
{
	for(IManipulator * &mp : mManipulators)
		delete mp;
}

void ColumnMappingItem::init(void)
{
	mSourceColumn = NULL;
	mTargetColumn = NULL;
}

ColumnMappingItem &ColumnMappingItem::operator=(ColumnMappingItem const &oSource)
{
	copy(oSource);
	return *this;
}

void ColumnMappingItem::copy(ColumnMappingItem const &oSource)
{
	setSourceColumn(oSource.getSourceColumn());
	setTargetColumn(oSource.getTargetColumn());
	setManipulators(oSource.mManipulators);
}

DatabaseColumn *ColumnMappingItem::getSourceColumn(void) const
{
	return mSourceColumn;
}

void ColumnMappingItem::setSourceColumn(DatabaseColumn *oColumn)
{
	mSourceColumn = oColumn;
}

DatabaseColumn *ColumnMappingItem::getTargetColumn(void) const
{
	return mTargetColumn;
}

void ColumnMappingItem::setTargetColumn(DatabaseColumn *oColumn)
{
	mTargetColumn = oColumn;
}

int ColumnMappingItem::hasManipulator(IManipulator *oManipulator)
{
	int i = -1;
	for(IManipulator * &mp : mManipulators)
	{
		i++;
		if(mp == oManipulator)
			return i;
	}

	return -1;
}

void ColumnMappingItem::clearManipulators(void)
{
	while(mManipulators.size() > 0)
	{
		IManipulator *mp = mManipulators[0];
		mManipulators.erase(mManipulators.begin());
		delete mp;
	}

	mManipulators.clear();
}

std::vector<IManipulator *> const &ColumnMappingItem::getManipulators(void) const
{
	return mManipulators;
}

void ColumnMappingItem::setManipulators(std::vector<IManipulator *> const &oManipulators)
{
	clearManipulators();
	for(IManipulator * const &mp : oManipulators)
		mManipulators.push_back(mp->duplicate());
}

void ColumnMappingItem::addManipulator(IManipulator *oManipulator)
{
	if(!oManipulator)
		return;

	if(hasManipulator(oManipulator) != -1)
		return;

	mManipulators.push_back(oManipulator->duplicate());
}

void ColumnMappingItem::removeManipulator(IManipulator *oManipulator)
{
	if(!oManipulator)
		return;

	int i = hasManipulator(oManipulator);
	if(i == -1)
		return;

	delete mManipulators[i];
	mManipulators.erase(mManipulators.begin());
}

void ColumnMappingItem::prepare(void)
{
	for(IManipulator * const &mp : mManipulators)
		mp->prepare();
}

StdString *ColumnMappingItem::format(StdString *oValue, bool bPreview)
{
	for(IManipulator * const &mp : mManipulators)
		oValue = mp->format(oValue, bPreview);

	return oValue;
}

void ColumnMappingItem::writeColumn(QSettings &oProfile, StdString const &oKey, DatabaseColumn *oColumn, bool bWriteType) const
{
	auto key = spt::string::toQt(oKey);

	if(!oColumn)
	{
		oProfile.setValue(key, "");
		return;
	}

	oProfile.setValue(key, spt::string::toQt(oColumn->getName()));
	if(bWriteType)
		oProfile.setValue(key+"_type", oColumn->getType());
}

bool ColumnMappingItem::loadProfile(QSettings &oProfile, StdString const &oKey, std::vector<DatabaseColumn *> &oColumns)
{
	auto key = spt::string::toQt(oKey);

	// Update the manipulators with the current set of columns
	for(IManipulator * const &ref : gReferences)
		ref->setSourceColumns(oColumns);

	StdString nm = spt::string::fromQt(oProfile.value(key+"_src", "").toString());
	DatabaseColumn *col = NULL;
	if(nm.length() > 0)
	{
		col = new DatabaseColumn();
		col->setName(nm);
	}
	setSourceColumn(col);

	nm = spt::string::fromQt(oProfile.value(key+"_tgt", "").toString());
	col = NULL;
	if(nm.length() > 0)
	{
		col = new DatabaseColumn();
		col->setName(nm);
		int t = oProfile.value(key+"_tgt_type", "1").toInt();
		col->setType(static_cast<spt::db::DataType>(t));
	}
	setTargetColumn(col);

	int n = oProfile.value(key+"_manipulators", "0").toInt();
	for(int i = 0; i < n; i++)
	{
		StdString uuid = spt::string::fromQt(oProfile.value(key+"_"+spt::string::toQt(spt::string::toString(i))+"_manipulator_id", "").toString());
		if(uuid.length() == 0)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "LoadProfile", "Inconsistent profile.");
			return false;
		}

		IManipulator *mp = NULL;
		for(IManipulator * const &ref : gReferences)
		{
			if(ref->getId() == uuid)
			{
				mp = ref->duplicate();
				break;
			}
		}

		if(mp == NULL)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "LoadProfile", "The manipulator with the id "+uuid+" doesn't exist");
			return false;
		}

		mp->loadProfile(oProfile, oKey+"_"+spt::string::toString(i)+"_"+uuid);
		addManipulator(mp);
	}

	return true;
}

void ColumnMappingItem::saveProfile(QSettings &oProfile, StdString const &oKey) const
{
	writeColumn(oProfile, oKey+"_src", getSourceColumn(), false);
	writeColumn(oProfile, oKey+"_tgt", getTargetColumn(), true);
	oProfile.setValue(spt::string::toQt(oKey+"_manipulators"), mManipulators.size());
	int i = 0;
	for(IManipulator * const &mp : mManipulators)
	{
		StdString uuid = mp->getId();
		StdString k = oKey + "_" + spt::string::toString(i);
		oProfile.setValue(spt::string::toQt(k+"_manipulator_id"), spt::string::toQt(uuid));
		mp->saveProfile(oProfile, k+"_"+uuid);
		i++;
	}
}

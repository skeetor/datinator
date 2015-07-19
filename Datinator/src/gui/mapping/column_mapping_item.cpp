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

Q_DECLARE_METATYPE(IManipulator *)

extern QList<IManipulator *> gReferences;

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
		IManipulator *mp = mManipulators.takeAt(0);
		delete mp;
	}

	mManipulators.clear();
}

QList<IManipulator *> const &ColumnMappingItem::getManipulators(void) const
{
	return mManipulators;
}

void ColumnMappingItem::setManipulators(QList<IManipulator *> const &oManipulators)
{
	clearManipulators();
	for(IManipulator * const &mp : oManipulators)
		mManipulators.append(mp->duplicate());
}

void ColumnMappingItem::addManipulator(IManipulator *oManipulator)
{
	if(!oManipulator)
		return;

	if(hasManipulator(oManipulator) != -1)
		return;

	mManipulators.append(oManipulator->duplicate());
}

void ColumnMappingItem::removeManipulator(IManipulator *oManipulator)
{
	if(!oManipulator)
		return;

	int i = hasManipulator(oManipulator);
	if(i == -1)
		return;

	delete mManipulators.takeAt(i);
}

void ColumnMappingItem::prepare(void)
{
	for(IManipulator * const &mp : mManipulators)
		mp->prepare();
}

QString *ColumnMappingItem::format(QString *oValue, bool bPreview)
{
	for(IManipulator * const &mp : mManipulators)
		oValue = mp->format(oValue, bPreview);

	return oValue;
}

void ColumnMappingItem::writeColumn(QSettings &oProfile, QString const &oKey, DatabaseColumn *oColumn, bool bWriteType) const
{
	if(!oColumn)
	{
		oProfile.setValue(oKey, "");
		return;
	}

	oProfile.setValue(oKey, oColumn->getName());
	if(bWriteType)
		oProfile.setValue(oKey+"_type", oColumn->getType());
}

bool ColumnMappingItem::loadProfile(QSettings &oProfile, QString const &oKey, QList<DatabaseColumn *> &oColumns)
{
	// Update the manipulators with the current set of columns
	for(IManipulator * const &ref : gReferences)
		ref->setSourceColumns(oColumns);

	QString nm = oProfile.value(oKey+"_src", "").toString();
	DatabaseColumn *col = NULL;
	if(nm.length() > 0)
	{
		col = new DatabaseColumn();
		col->setName(nm);
	}
	setSourceColumn(col);

	nm = oProfile.value(oKey+"_tgt", "").toString();
	col = NULL;
	if(nm.length() > 0)
	{
		col = new DatabaseColumn();
		col->setName(nm);
		int t = oProfile.value(oKey+"_tgt_type", "1").toInt();
		col->setType(static_cast<supportlib::db::DataType>(t));
	}
	setTargetColumn(col);

	int n = oProfile.value(oKey+"_manipulators", "0").toInt();
	for(int i = 0; i < n; i++)
	{
		QString uuid = oProfile.value(oKey+"_"+QString::number(i)+"_manipulator_id", "").toString();
		if(uuid.length() == 0)
		{
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, "LoadProfile", "Inconsistent profile.");
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
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, "LoadProfile", "The manipulator with the id "+uuid+" doesn't exist");
			return false;
		}

		mp->loadProfile(oProfile, oKey+"_"+QString::number(i)+"_"+uuid);
		addManipulator(mp);
	}

	return true;
}

void ColumnMappingItem::saveProfile(QSettings &oProfile, QString const &oKey) const
{
	writeColumn(oProfile, oKey+"_src", getSourceColumn(), false);
	writeColumn(oProfile, oKey+"_tgt", getTargetColumn(), true);
	oProfile.setValue(oKey+"_manipulators", mManipulators.size());
	int i = 0;
	for(IManipulator * const &mp : mManipulators)
	{
		QString uuid = mp->getId();
		QString k = oKey+"_"+QString::number(i);
		oProfile.setValue(k+"_manipulator_id", uuid);
		mp->saveProfile(oProfile, k+"_"+uuid);
		i++;
	}
}

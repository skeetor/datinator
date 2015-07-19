/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef COLUMN_MAPPING_ITEM_H
#define COLUMN_MAPPING_ITEM_H

#include <QtCore/QList>

#include "datinator_types.h"

class IManipulator;
class ColumnMappingButton;
class QSettings;

typedef enum
{
	SOURCE,
	TARGET,
	MANIPULATOR,
	MAX,
} Columns;

class ColumnMappingItem
{
public:
	ColumnMappingItem(void);
	ColumnMappingItem(ColumnMappingItem const &oSource);
	virtual ~ColumnMappingItem(void);

	virtual void copy(ColumnMappingItem const &oSource);
	virtual ColumnMappingItem &operator=(ColumnMappingItem const &oSource);

	DatabaseColumn *getSourceColumn(void) const;
	void setSourceColumn(DatabaseColumn *oColumn);

	DatabaseColumn *getTargetColumn(void) const;
	void setTargetColumn(DatabaseColumn *oColumn);

	QList<IManipulator *> const &getManipulators(void) const;
	void setManipulators(QList<IManipulator *> const &oManipulators);
	void clearManipulators(void);
	void addManipulator(IManipulator *oManipulator);
	void removeManipulator(IManipulator *oManipulator);

	/**
	 * Prepare the manipulators for operation.
	 */
	void prepare(void);

	/**
	 * Apply the manipulation to the data.
	 */
	QString *format(QString *oValue, bool bPreview = false);

	bool loadProfile(QSettings &oProfile, QString const &oKey, QList<DatabaseColumn *> &oColumns);
	void saveProfile(QSettings &oProfile, QString const &oKey) const;

protected:
	int hasManipulator(IManipulator *oManipulator);
	void writeColumn(QSettings &oProfile, QString const &oKey, DatabaseColumn *oColumn, bool bWriteType) const;

private:
	void init(void);

private:
	DatabaseColumn *mSourceColumn;
	DatabaseColumn *mTargetColumn;
	QList<IManipulator *> mManipulators;
};

#endif // COLUMN_MAPPING_ITEM_H

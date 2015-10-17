/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef COLUMN_MAPPING_ITEM_H
#define COLUMN_MAPPING_ITEM_H

#include <vector>

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

	std::vector<IManipulator *> const &getManipulators(void) const;
	void setManipulators(std::vector<IManipulator *> const &oManipulators);
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
	StdString *format(StdString *oValue, bool bPreview = false);

	bool loadProfile(QSettings &oProfile, StdString const &oKey, std::vector<DatabaseColumn *> &oColumns);
	void saveProfile(QSettings &oProfile, StdString const &oKey) const;

protected:
	int hasManipulator(IManipulator *oManipulator);
	void writeColumn(QSettings &oProfile, StdString const &oKey, DatabaseColumn *oColumn, bool bWriteType) const;

private:
	void init(void);

private:
	DatabaseColumn *mSourceColumn;
	DatabaseColumn *mTargetColumn;
	std::vector<IManipulator *> mManipulators;
};

#endif // COLUMN_MAPPING_ITEM_H

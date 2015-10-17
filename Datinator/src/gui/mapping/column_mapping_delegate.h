/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef _COLUMN_MAPPING_DELEGATE_H
#define _COLUMN_MAPPING_DELEGATE_H

#include <vector>
#include <QtWidgets/QStyledItemDelegate>

#include <support/patterns/observer.h>
#include "support/db/dbcolumn.h"

#include "manipulator/imanipulator.h"

class ColumnMappingView;
class ColumnMappingModel;
class ColumnMappingButton;

class ColumnMappingDelegate
	: public QStyledItemDelegate
{
public:
	ColumnMappingDelegate(std::vector<IManipulator *> oManipulators, ColumnMappingModel *oModel, ColumnMappingView *oView, QObject *oParent = 0);
	virtual ~ColumnMappingDelegate(void);

	virtual QWidget *createEditor(QWidget *oParent, const QStyleOptionViewItem &oOption, const QModelIndex &oIndex) const;
	virtual void setEditorData(QWidget *oEditor, const QModelIndex &oIndex) const;
	virtual void setModelData(QWidget *oEditor, QAbstractItemModel *oModel, const QModelIndex &oIndex) const;

public:
	void setSourceColumns(std::vector<DatabaseColumn *> const &oColumns);
	void setTargetColumns(std::vector<DatabaseColumn *> const &oColumns);
	void setDispatcher(Dispatcher<std::vector<DatabaseColumn *> *> *oSource, Dispatcher<std::vector<DatabaseColumn *> *> *oTarget);

private:
	ColumnMappingModel *mModel;
	ColumnMappingView *mView;
	Dispatcher<std::vector<DatabaseColumn *> *> *mSourceDispatcher;
	Dispatcher<std::vector<DatabaseColumn *> *> *mTargetDispatcher;
    std::vector<DatabaseColumn *> mSourceColumns;
    std::vector<DatabaseColumn *> mTargetColumns;
    std::vector<ColumnMappingButton *> *mButtonList;
};

#endif // _COLUMN_MAPPING_DELEGATE_H

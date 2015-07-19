/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef COLUMN_MAPPING_DELEGATE_H
#define COLUMN_MAPPING_DELEGATE_H

#include <QtCore/QList>
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
	ColumnMappingDelegate(QList<IManipulator *> oManipulators, ColumnMappingModel *oModel, ColumnMappingView *oView, QObject *oParent = 0);
	virtual ~ColumnMappingDelegate(void);

	virtual QWidget *createEditor(QWidget *oParent, const QStyleOptionViewItem &oOption, const QModelIndex &oIndex) const;
	virtual void setEditorData(QWidget *oEditor, const QModelIndex &oIndex) const;
	virtual void setModelData(QWidget *oEditor, QAbstractItemModel *oModel, const QModelIndex &oIndex) const;

public:
	void setSourceColumns(QList<DatabaseColumn *> const &oColumns);
	void setTargetColumns(QList<DatabaseColumn *> const &oColumns);
	void setDispatcher(Dispatcher<QList<DatabaseColumn *> *> *oSource, Dispatcher<QList<DatabaseColumn *> *> *oTarget);

private:
	ColumnMappingModel *mModel;
	ColumnMappingView *mView;
	Dispatcher<QList<DatabaseColumn *> *> *mSourceDispatcher;
	Dispatcher<QList<DatabaseColumn *> *> *mTargetDispatcher;
    QList<DatabaseColumn *> mSourceColumns;
    QList<DatabaseColumn *> mTargetColumns;
    QList<ColumnMappingButton *> *mButtonList;
};

#endif // COLUMN_MAPPING_DELEGATE_H

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef COLUMN_MAPPING_MODEL_H
#define COLUMN_MAPPING_MODEL_H

#include <QtGui/QStandardItemModel>

#include "gui/mapping/column_mapping_item.h"

class ColumnMappingModel : public QStandardItemModel
{
public:
	ColumnMappingModel(void);
	virtual ~ColumnMappingModel(void);

	ColumnMappingItem *getRowItem(int nRow) const;
	void setRowItem(int nRow, ColumnMappingItem const &oItem);
	void setRowcount(int nRows);

	void setSourceColumn(int nRow, DatabaseColumn *oColumn);
	DatabaseColumn *getSourceColumn(int nRow) const;
	void clearSourceColumns(void);

	void setTargetColumn(int nRow, DatabaseColumn *oColumn);
	DatabaseColumn *getTargetColumn(int nRow) const;
	void clearTargetColumns(void);

	void setManipulators(int nRow, std::vector<IManipulator *> const &oManipulators);
	std::vector<IManipulator *> getManipulators(int nRow) const;
	void clearManipulators(void);

	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(QModelIndex const &oIndex, QVariant const &oValue, int nRole) override;
	bool insertRows(int nRow, int nCount, const QModelIndex &oParent = QModelIndex()) override;
    bool removeRows(int nRow, int nCount, const QModelIndex &oParent = QModelIndex()) override;

protected:
	void createRowItem(int nRow);
};

#endif // COLUMN_MAPPING_MODEL_H

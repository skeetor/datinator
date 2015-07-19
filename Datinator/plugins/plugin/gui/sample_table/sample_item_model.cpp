/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "plugin/gui/datatype_string.h"
#include "plugin/gui/sample_table/sample_item_model.h"
#include "support/unicode/unicode_types.h"

SampleItemModel::SampleItemModel(void)
: QStandardItemModel()
{
	mShowDatatypes = true;
}

SampleItemModel::~SampleItemModel(void)
{
}

void SampleItemModel::showDatatypes(bool bShowDatatypes)
{
	mShowDatatypes = bShowDatatypes;
}

void SampleItemModel::clearRows(void)
{
	removeRows(0, rowCount());
}

Qt::ItemFlags SampleItemModel::flags(const QModelIndex &oIndex) const
{
	Qt::ItemFlags fl = QStandardItemModel::flags(oIndex);
	if(!oIndex.isValid())
		return fl;

	if(mShowDatatypes)
	{
		int r = oIndex.row();
		int s = rowCount()-1;
		if(r >= s)
			fl |= Qt::ItemIsEditable;
	}

	return fl;
}

void SampleItemModel::setColumns(QList<DatabaseColumn *> const &oColumns)
{
	clear();
	mColumns = oColumns;
	int n = oColumns.size();
	if(n == 0)
		n++;
	insertColumns(0, n);
	n = 0;
	for(DatabaseColumn * const &col : mColumns)
		setHeaderData(n++, Qt::Horizontal, col->getName());
}

void SampleItemModel::setRows(QList<QList<QString>> const &oSampleRows)
{
	removeRows(0, rowCount());

	int n = oSampleRows.size();
	insertRows(0, n);

	QList<QList<QString>> rows = oSampleRows;
	if(mShowDatatypes)
	{
		QList<QString> sl;
		for(DatabaseColumn * const &col  : mColumns)
			sl.append(TypeStringProvider.toString(col->getType()));

		rows.append(sl);
	}

	n = 0;
	for(QList<QString> const &row : rows)
		setRow(n++, row);
}

void SampleItemModel::setRow(int nRow, QList<QString> const &oData)
{
	QStandardItem *item = NULL;

	int i = 0;
	for(QString const &col : oData)
	{
		item = new QStandardItem(col);
		setItem(nRow, i++, item);
	}
}

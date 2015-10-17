/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "plugin/gui/datatype_string.h"
#include "plugin/gui/sample_table/sample_item_model.h"
#include "support/unicode/unicode_types.h"
#include "support/helper/string.h"

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

void SampleItemModel::setColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	clear();
	mColumns = oColumns;
	int n = oColumns.size();
	if(n == 0)
		n++;
	insertColumns(0, n);
	n = 0;
	for(DatabaseColumn * const &col : mColumns)
		setHeaderData(n++, Qt::Horizontal, spt::string::toQt(col->getName()));
}

void SampleItemModel::setRows(std::vector<std::vector<StdString>> const &oSampleRows)
{
	removeRows(0, rowCount());

	int n = oSampleRows.size();
	insertRows(0, n);

	std::vector<std::vector<StdString>> rows = oSampleRows;
	if(mShowDatatypes)
	{
		std::vector<StdString> sl;
		for(DatabaseColumn * const &col  : mColumns)
			sl.push_back(TypeStringProvider.toString(col->getType()));

		rows.push_back(sl);
	}

	n = 0;
	for(std::vector<StdString> const &row : rows)
		setRow(n++, row);
}

void SampleItemModel::setRow(int nRow, std::vector<StdString> const &oData)
{
	QStandardItem *item = NULL;

	int i = 0;
	for(StdString const &col : oData)
	{
		item = new QStandardItem(spt::string::toQt(col));
		setItem(nRow, i++, item);
	}
}

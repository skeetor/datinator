/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QString>

#include "gui/mapping/column_mapping_model.h"
#include "support/db/dbcolumn.h"
#include "manipulator/imanipulator.h"

#include "support/helper/string.h"

Q_DECLARE_METATYPE(DatabaseColumn *)
Q_DECLARE_METATYPE(ColumnMappingItem *)
Q_DECLARE_METATYPE(IManipulator *)

ColumnMappingModel::ColumnMappingModel(void)
: QStandardItemModel()
{
	setHorizontalHeaderItem(Columns::SOURCE, new QStandardItem(QString("Source")));
	setHorizontalHeaderItem(Columns::TARGET, new QStandardItem(QString("Target")));
	setHorizontalHeaderItem(Columns::MANIPULATOR, new QStandardItem(QString("Manipulator")));
}

ColumnMappingModel::~ColumnMappingModel(void)
{
}

Qt::ItemFlags ColumnMappingModel::flags(const QModelIndex &oIndex) const
{
	Qt::ItemFlags fl = QStandardItemModel::flags(oIndex);
	if (!oIndex.isValid())
		return fl;

	// We want the modle to be editable, but it shouldn't be changeable, so we
	// will disable storing of edited data.
	fl |= Qt::ItemIsEditable;

	return fl;
}

bool ColumnMappingModel::setData(QModelIndex const &oIndex, QVariant const &oValue, int nRole)
{
	if(!oIndex.isValid())
		return QStandardItemModel::setData(oIndex, oValue, nRole);

	if(nRole != Qt::EditRole)
		return QStandardItemModel::setData(oIndex, oValue, nRole);

	int c = oIndex.column();
	int r = oIndex.row();
	if(c == Columns::SOURCE || c == Columns::TARGET)
	{
		DatabaseColumn *col = oValue.value<DatabaseColumn *>();
		if(c == Columns::SOURCE)
			setSourceColumn(r, col);
		else
			setTargetColumn(r, col);
	}
	else if(c == Columns::MANIPULATOR)
	{
		std::vector<IManipulator *>ml = oValue.value<std::vector<IManipulator *>>();
		setManipulators(r, ml);
	}

	return true;
}

bool ColumnMappingModel::insertRows(int nRow, int nCount, const QModelIndex &oParent)
{
	bool rc = QStandardItemModel::insertRows(nRow, nCount, oParent);

	for(int i = 0; i < nCount; i++)
		createRowItem(nRow+i);

	return rc;
}

bool ColumnMappingModel::removeRows(int nRow, int nCount, const QModelIndex &oParent)
{
	for(int i = 0; i < nCount; i++)
	{
		ColumnMappingItem *mi = getRowItem(nRow+i);
		if(mi == NULL)
			continue;
		mi->clearManipulators();
		delete mi;
	}

	return QStandardItemModel::removeRows(nRow, nCount, oParent);
}

void ColumnMappingModel::setRowcount(int nRows)
{
	if(nRows < 1)
		nRows = 1;

	int r = rowCount();
	if(nRows == r)
		return;

	// If the number of rows doesn't change, we don't need to do anything here.
	int total = abs(r-nRows);
	if(total == 0)
		return;

	if(nRows > r)
		insertRows(r, total);
	else
		removeRows(r-total, total);
}

void ColumnMappingModel::createRowItem(int nRow)
{
	ColumnMappingItem *mi;

	QStandardItem *si = item(nRow, Columns::SOURCE);
	if(si == NULL)
	{
		int n = columnCount();
		for(int i = 0; i < n; i++)
		{
			si = new QStandardItem();
			setItem(nRow, i, si);
		}
		si = item(nRow, Columns::SOURCE);
	}

	QVariant v = si->data();
	mi = v.value<ColumnMappingItem *>();

	if(mi == NULL)
	{
		mi = new ColumnMappingItem();
		QStandardItem *si = item(nRow, Columns::SOURCE);
		QVariant v;
		v.setValue(mi);
		si->setData(v);
	}
}

ColumnMappingItem *ColumnMappingModel::getRowItem(int nRow) const
{
	ColumnMappingItem *mi;

	QStandardItem *si = item(nRow, Columns::SOURCE);
	QVariant v = si->data();
	mi = v.value<ColumnMappingItem *>();

	return mi;
}

void ColumnMappingModel::setRowItem(int nRow, ColumnMappingItem const &oItem)
{
	setSourceColumn(nRow, oItem.getSourceColumn());
	setTargetColumn(nRow, oItem.getTargetColumn());
	setManipulators(nRow, oItem.getManipulators());
}

void ColumnMappingModel::setSourceColumn(int nRow, DatabaseColumn *oColumn)
{
	ColumnMappingItem *mi = getRowItem(nRow);
	QString s;
	QStandardItem *it = item(nRow, Columns::SOURCE);
	mi->setSourceColumn(oColumn);
	if(oColumn)
		s = spt::string::toQt(oColumn->getName());
	it->setText(s);
	setItem(nRow, Columns::SOURCE, it);
}

DatabaseColumn *ColumnMappingModel::getSourceColumn(int nRow) const
{
	ColumnMappingItem *mi = getRowItem(nRow);
	if(mi == NULL)
		return NULL;

	return mi->getSourceColumn();
}

void ColumnMappingModel::clearSourceColumns(void)
{
	int n = rowCount();

	for(int i = 0; i < n; i++)
		setSourceColumn(i, NULL);
}

void ColumnMappingModel::setTargetColumn(int nRow, DatabaseColumn *oColumn)
{
	ColumnMappingItem *mi = getRowItem(nRow);
	QString s;
	QStandardItem *it = item(nRow, Columns::TARGET);
	mi->setTargetColumn(oColumn);
	if(oColumn)
		s = spt::string::toQt(oColumn->getName());
	it->setText(s);
	setItem(nRow, Columns::TARGET, it);
}

DatabaseColumn *ColumnMappingModel::getTargetColumn(int nRow) const
{
	ColumnMappingItem *mi = getRowItem(nRow);
	if(mi == NULL)
		return NULL;

	return mi->getTargetColumn();
}

void ColumnMappingModel::clearTargetColumns(void)
{
	int n = rowCount();

	for(int i = 0; i < n; i++)
		setTargetColumn(i, NULL);
}

void ColumnMappingModel::setManipulators(int nRow, std::vector<IManipulator *> const &oManipulators)
{
	ColumnMappingItem *mi = getRowItem(nRow);
	QStandardItem *it = item(nRow, Columns::MANIPULATOR);
	mi->setManipulators(oManipulators);

	if(oManipulators.size() > 0)
	{
		mi->prepare();
		StdString *s = new StdString("*VALUE*");
		s = mi->format(s, true);
		if(s)
		{
			it->setText(spt::string::toQt(*s));
			delete s;
		}
		else
			it->setText("[null]");
	}
	else
		it->setText("");

	setItem(nRow, Columns::MANIPULATOR, it);
}

std::vector<IManipulator *> ColumnMappingModel::getManipulators(int nRow) const
{
	ColumnMappingItem *mi = getRowItem(nRow);
	return mi->getManipulators();
}

void ColumnMappingModel::clearManipulators(void)
{
	int n = rowCount();

	std::vector<IManipulator *>ml;
	for(int i = 0; i < n; i++)
		setManipulators(i, ml);
}

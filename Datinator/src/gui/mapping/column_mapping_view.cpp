/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>

#include "gui/mapping/column_mapping_view_qt.moc"
#include "manipulator/imanipulator.h"

#include "support/helper/string.h"

Q_DECLARE_METATYPE(IManipulator *)

ColumnMappingView::ColumnMappingView(QWidget *oParent)
: QTableView(oParent)
{
	mReadOnly = false;

	mDelegate = new ColumnMappingDelegate(mManipulators, &mItemModel, this, this);

	mDelegate->setDispatcher(&mSourceColumns, &mTargetColumns);
	setItemDelegate(mDelegate);

	verticalHeader()->setVisible(true);
	verticalHeader()->setDefaultSectionSize(20);
	horizontalHeader()->setVisible(true);
	horizontalHeader()->setStretchLastSection(true);
	setModel(&mItemModel);
}

ColumnMappingView::~ColumnMappingView(void)
{
// TODO: Deleted by Qt?
//	delete mDelegate;
//	for(IManipulator * &mp : mManipulators)
//		delete mp;
}

void ColumnMappingView::onRowcount(int nRows)
{
	setRowcount(nRows);
}

int ColumnMappingView::rowCount(void)
{
	return mItemModel.rowCount();
}

ColumnMappingItem *ColumnMappingView::rowItem(int nRow)
{
	return mItemModel.getRowItem(nRow);
}

std::vector<ColumnMappingItem> ColumnMappingView::rowItems(void)
{
	int rows = rowCount();
	std::vector<ColumnMappingItem>l;
	ColumnMappingItem m;
	for(int i = 0; i < rows; i++)
	{
		ColumnMappingItem *mi = rowItem(i);
		if(mi)
			l.push_back(*mi);
		else
			l.push_back(m);
	}

	return l;
}

QModelIndexList ColumnMappingView::selectedRows(std::vector<ColumnMappingItem> *oItems)
{
	QModelIndexList il = selectionModel()->selectedRows();

	if(oItems)
	{
		ColumnMappingItem m;
		for(QModelIndex const &i : il)
		{
			ColumnMappingItem *mi = rowItem(i.row());
			if(mi)
				oItems->push_back(*mi);
			else
				oItems->push_back(m);
		}
	}

	return il;
}

void ColumnMappingView::setRowcount(int nRows)
{
	if(nRows < 1)
		nRows = 1;

	mItemModel.setRowcount(nRows);
	updateWidgetView();
	updateDefaultSelection(Columns::SOURCE, mSourceColumns);
	updateDefaultSelection(Columns::TARGET, mTargetColumns);
	notifyRowcountListeners(nRows);
}

void ColumnMappingView::updateWidgetView(void)
{
	int columns = mItemModel.columnCount();
	int rows = mItemModel.rowCount();

	for(int row = 0; row < rows; row++)
	{
		for(int column = 0; column < columns; column++)
		{
			QModelIndex index = mItemModel.index(row, column);
			openPersistentEditor(index);
		}
	}
}

void ColumnMappingView::clearRows(void)
{
//	mItemModel.clearRows();
}

std::vector<DatabaseColumn *> ColumnMappingView::getSourceColumns(void)
{
	return mSourceColumns.getColumns();
}

void ColumnMappingView::setSourceColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	mItemModel.clearSourceColumns();

	mSourceColumns = oColumns;  // This assignment will trigger the notification to the listeners.
	mDelegate->setSourceColumns(mSourceColumns);

	// If the targetlist is empty, we force an update with the
	// the empty list, which will load the sourcelist as default.
	// If the current list was set as the default, we replace the current
	// also with the new list.
	if(mTargetColumns.list().size() == 0 || mTargetColumns.isDefault())
		setTargetColumns(mTargetColumns, true);
	else
		updateWidgetView();

	updateDefaultSelection(Columns::SOURCE, mSourceColumns);
}

std::vector<DatabaseColumn *> ColumnMappingView::getTargetColumns()
{
	return mTargetColumns.getColumns();
}

void ColumnMappingView::setTargetColumns(std::vector<DatabaseColumn *> const &oColumns, bool bDefault)
{
	mItemModel.clearTargetColumns();
	mItemModel.clearManipulators();

	std::vector<DatabaseColumn *>l;

  	if(oColumns.size() == 0 || bDefault == true)
	{
		if(!isReadOnly())
			l = mSourceColumns;

		bDefault = true;
	}
	else
		l = oColumns;

	mTargetColumns.copy(l, true);
	mTargetColumns.setDefault(bDefault);
	mDelegate->setTargetColumns(mTargetColumns);
	setRowcount(mTargetColumns.list().size());
	updateDefaultSelection(Columns::TARGET, mTargetColumns);
}

void ColumnMappingView::updateDefaultSelection(int nColumn, std::vector<DatabaseColumn *> const &oColumns)
{
	if(nColumn == Columns::SOURCE)
		mItemModel.clearSourceColumns();
	else
		mItemModel.clearTargetColumns();

	int rows = mItemModel.rowCount();
	int n = oColumns.size();
	for(int row = 0; row < rows; row++)
	{
		DatabaseColumn *col = NULL;
		if(row < n)
			col = oColumns.at(row);

		if(nColumn == Columns::SOURCE)
			mItemModel.setSourceColumn(row, col);
		else if(nColumn == Columns::TARGET)
			mItemModel.setTargetColumn(row, col);
	}
}

void ColumnMappingView::clearSelection(void)
{
	selectionModel()->clearSelection();
}

void ColumnMappingView::removeSelected(void)
{
	QItemSelectionModel *sm = selectionModel();

	QModelIndexList indexes;
	while((indexes = sm->selectedIndexes()).size())
		mItemModel.removeRow(indexes.first().row());
}

void ColumnMappingView::moveUp(int nCount)
{
	QItemSelectionModel *sm = selectionModel();

	QModelIndexList indexes = sm->selectedRows();
	QItemSelection sel;

	for(QModelIndex const &index : indexes)
	{
		int r = index.row();
		int nr = r - nCount;
		if(nr < 0)
			return;

		ColumnMappingItem old_row = *mItemModel.getRowItem(r);
		ColumnMappingItem new_row = *mItemModel.getRowItem(nr);
		mItemModel.setRowItem(r, new_row);
		mItemModel.setRowItem(nr, old_row);
		QModelIndex mi = mItemModel.index(nr, 0);
		sel.select(mi, mi);
	}
	sm->clearSelection();
	sm->select(sel, QItemSelectionModel::Select|QItemSelectionModel::Rows);
}

void ColumnMappingView::moveDown(int nCount)
{
	QItemSelection sel;
	QItemSelectionModel *sm = selectionModel();
	QModelIndexList selected = sm->selectedRows();
	int mx = selected.size();
	int n = mItemModel.rowCount()-1;
	std::vector<int> selection;

	for(int i = mx-1; i >= 0; i--)
	{
		QModelIndex index = selected[i];
		int r = index.row();
		int nr = r + nCount;
		if(nr > n)
			return;

		ColumnMappingItem old_row = *mItemModel.getRowItem(r);
		ColumnMappingItem new_row = *mItemModel.getRowItem(nr);
		mItemModel.setRowItem(r, new_row);
		mItemModel.setRowItem(nr, old_row);
		selection.insert(selection.begin(), nr);
	}

	// The selection range can not deal with ranges in a reversed order, so we have to
	// make sure that the selection indexes are from top to bottom.
	for(int i : selection)
	{
		QModelIndex mi = mItemModel.index(i, 0);
		sel.select(mi, mi);
	}
	sm->clearSelection();
	sm->select(sel, QItemSelectionModel::Select|QItemSelectionModel::Rows);
}

bool ColumnMappingView::loadProfile(QSettings &oProfile)
{
	std::vector<ColumnMappingItem> items;
	StdString const s = "item_";
	int n = rowCount();
	std::vector<DatabaseColumn *> columns = getSourceColumns();

	/**
	 * For each row that we loaded, we have to look it up in the current
	 * list, and select it, to make sure that all attributes are from the source are preserved.
	 * The user could have changed the datatype of the target, so we have to preserve it
	 * from the loaded definition.
	 */
	for(int i = 0; i < n; i++)
	{
		ColumnMappingItem ni;
		ni.loadProfile(oProfile, s+spt::string::toString(i), columns);
		DatabaseColumn *col = ni.getSourceColumn();
		DatabaseColumn *rep = NULL;
		int ind = -1;
		if(col)
		{
			int ind = mSourceColumns.find(col);
			if(ind != -1)
				rep = mSourceColumns[ind];
			else
			{
				ind = mSourceColumns.addColumn(col);
				rep = mSourceColumns[ind];
			}
			delete col;
		}

		ni.setSourceColumn(rep);

		col = ni.getTargetColumn();
		rep = NULL;

		if(col)
		{
			ind = mTargetColumns.find(col);
			if(ind != -1)
				rep = mTargetColumns[ind];
			else
			{
				ind = mTargetColumns.addColumn(col);
				rep = mTargetColumns[ind];
			}

			if(col)
			{
				if(rep)
					rep->setType(col->getType());

				delete col;
			}
		}

		ni.setTargetColumn(rep);
		items.push_back(ni);
	}

	// Notify the comboboxes with the new set of columns (if any have been added)
	mSourceColumns.notifyColumnListeners();
	mTargetColumns.notifyColumnListeners();

	int i = -1;
	for(ColumnMappingItem &item : items)
	{
		i++;

		// For some reason the model doesn't update the comboboxes if they have the old
		// value, and because of the notification above the boxes will show empty values.
		// So as a hack, we clear the current values, and then set the new values, to force a refresh.
		ColumnMappingItem empty;
		mItemModel.setRowItem(i, empty);
		mItemModel.setRowItem(i, item);
	}

	return true;
}

void ColumnMappingView::saveProfile(QSettings &oProfile)
{
	std::vector<ColumnMappingItem> items = rowItems();
	int i = -1;
	StdString const s = "item_";
	for(ColumnMappingItem const &mi : items)
		mi.saveProfile(oProfile, s+spt::string::toString(++i));
}

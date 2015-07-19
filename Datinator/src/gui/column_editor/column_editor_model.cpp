/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QDateTime>

#include "support/db/dbcolumn.h"

#include "plugin/gui/datatype_string.h"
#include "gui/column_editor/column_editor_model.h"

Q_DECLARE_METATYPE(DatabaseColumn *)

typedef enum
{
	COL_NAME,
	COL_DATATYPE,
	COL_MAX
} ItemColumns;

ColumnEditorModel::ColumnEditorModel(QObject *oParent)
: QStandardItemModel(oParent)
{
	setHorizontalHeaderItem(COL_NAME, new QStandardItem(QString("Name")));
	setHorizontalHeaderItem(COL_DATATYPE, new QStandardItem(QString("Datatype")));
	insertRow(0);
}

ColumnEditorModel::~ColumnEditorModel(void)
{
}

void ColumnEditorModel::clear(void)
{
	removeRows(0, rowCount());
}

Qt::ItemFlags ColumnEditorModel::flags(const QModelIndex &oIndex) const
{
	Qt::ItemFlags fl = QStandardItemModel::flags(oIndex);
	if (!oIndex.isValid())
		return fl;

	// We want the modle to be editable, but it shouldn't be changeable, so we
	// will disable storing of edited data.
	fl |= Qt::ItemIsEditable;

	return fl;
}

void ColumnEditorModel::addRowListener(Listener<bool, int, int> *oListener)
{
	Dispatcher<bool, int, int>::addListener(oListener);
}

void ColumnEditorModel::removeRowListener(Listener<bool, int, int> *oListener)
{
	Dispatcher<bool, int, int>::removeListener(oListener);
}

void ColumnEditorModel::notifyRowListeners(bool bInserted, int nRow, int nCount)
{
	Dispatcher<bool, int, int>::notify(bInserted, nRow, nCount);
}

bool ColumnEditorModel::insertRows(int row, int count, const QModelIndex &parent)
{
	bool rc = QStandardItemModel::insertRows(row, count, parent);
	if(rc)
		notifyRowListeners(true, row, count);

	return rc;
}

bool ColumnEditorModel::removeRows(int row, int count, const QModelIndex &parent)
{
	bool rc = QStandardItemModel::removeRows(row, count, parent);
	if(rc)
		notifyRowListeners(false, row, count);

	return rc;
}

bool ColumnEditorModel::insertRow(int row, const QModelIndex &parent)
{
	bool rc = QStandardItemModel::insertRow(row, parent);
	if(rc)
		notifyRowListeners(true, row, 1);

	return rc;
}

bool ColumnEditorModel::removeRow(int row, const QModelIndex &parent)
{
	bool rc = QStandardItemModel::removeRow(row, parent);
	if(rc)
		notifyRowListeners(false, row, 1);

	return rc;
}

void ColumnEditorModel::setColumns(QList<DatabaseColumn *> const &oColumns)
{
	clear();
	insertRows(0, oColumns.size()+1);
	QList<DatabaseColumn *> cols;
	for(DatabaseColumn * const col : cols)
		cols.append(col->duplicate());

	int i = 0;
	for(DatabaseColumn * const &col : oColumns)
	{
		putItem(i, col);
		i++;
	}
}

void ColumnEditorModel::putItem(int nRow, DatabaseColumn *oColumn)
{
	QStandardItem *it = item(nRow, 0);
	if(it == NULL)
		it = new QStandardItem();

	it->setText(oColumn->getName());
	QVariant v;
	v.setValue(oColumn);
	it->setData(v);
	setItem(nRow, 0, it);

	it = item(nRow, 1);
	if(it == NULL)
		it = new QStandardItem();
	it->setText(TypeStringProvider.toString(oColumn->getType()));
	setItem(nRow, 1, it);
}

void ColumnEditorModel::updateName(int nRow, QString const &oName)
{
	QStandardItem *it = item(nRow, 0);
	if(it == NULL)
		it = new QStandardItem();

	it->setText(oName);
	DatabaseColumn *col = columnItem(nRow);
	if(col == NULL)
		col = new DatabaseColumn();

	col->setName(oName);
	QVariant v;
	v.setValue(col);
	it->setData(v);
	setItem(nRow, 0, it);
}

void ColumnEditorModel::updateType(int nRow, QString const &oType)
{
	QStandardItem *it = item(nRow, 0);
	if(it == NULL)
		return;

	it->setText(oType);
	DatabaseColumn *col = columnItem(nRow);
	if(col == NULL)
		return;

	col->setType(TypeStringProvider.toType(oType));
	QVariant v;
	v.setValue(col);
	it->setData(v);
	setItem(nRow, 0, it);
}

void ColumnEditorModel::deleteRow(int nRow)
{
	DatabaseColumn *col = columnItem(nRow);
	removeRow(nRow);
	if(col)
		delete col;
}

DatabaseColumn *ColumnEditorModel::columnItem(int nRow)
{
	DatabaseColumn *col = NULL;
	QStandardItem *si = item(nRow, 0);
	if(si != NULL)
	{
		QVariant v = si->data();
		col = v.value<DatabaseColumn *>();
	}

	return col;
}

bool ColumnEditorModel::setData(QModelIndex const &oIndex, QVariant const &oValue, int nRole)
{
	if(!oIndex.isValid())
		return QStandardItemModel::setData(oIndex, oValue, nRole);

	QString s = oValue.value<QString>();
	bool rc;
	bool update = true;

	int n = rowCount()-1;
	int ir = oIndex.row();
	int ic = oIndex.column();
	if(ir == n)
	{
		// Add new empty line at the end if the last item has got some text
		if(ic == COL_NAME && s.size() > 0)
		{
			updateName(n, s);
			insertRow(n+1);
		}
	}
	else
	{
		if(ic == COL_NAME)
		{
			if(s.size() == 0)
			{
				n = oIndex.row();
				deleteRow(n);
				update = false;
			}
			else
				updateName(ir, s);
		}
		else
			updateType(ir, s);
	}

	if(update)
		rc = QStandardItemModel::setData(oIndex, oValue, nRole);
	else
		rc = false;

	return rc;
}

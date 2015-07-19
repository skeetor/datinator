//******************************************************************************
//
// DefaultItemModel (c) by Gerhard W. Gruber in 2014
//
// Table model for a view with fixed number of columns
//
//******************************************************************************

#ifndef DEFAULT_ITEM_MODEL_H
#define DEFAULT_ITEM_MODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>

template <typename T>
class DefaultItemModel : public QAbstractTableModel
{
public:
	DefaultItemModel(int nColumns)
	{
		mColumns = nColumns;
	}

	DefaultItemModel(int nColumns, QList<QVariant> const &oItems)
	{
		mColumns = nColumns;
		mItems = oItems;
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const
	{
		return mItems.count();
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const
	{
		return mColumns;
	}

	QVariant data(const QModelIndex& oIndex, int nRole = Qt::DisplayRole) const
	{
		if (!oIndex.isValid() || nRole != Qt::DisplayRole)
			return QVariant();

		// Return the data to which index points.
		return getItem(oIndex);
	}

	virtual QVariant getItem(const QModelIndex &oIndex) const
	{
		int row = oIndex.row();
		if(mItems.count() >= row)
			return QVariant();

		QVariant v = mItems.at(row);
		return getColumnItem(v.value<T>(), oIndex.row(), oIndex.column());
	}

	virtual QVariant getColumnItem(T const &oItem, int nRow, int nColumn) const
	{
		return QVariant();
	}

	virtual void addItem(T const &oItem)
	{
		QVariant v;
		v.setValue(oItem);

		mItems.append(v);
	}

	virtual void clearItems(void)
	{
		mItems.clear();
	}

private:
	int mColumns;
	QList<QVariant> mItems;
};

#endif // DEFAULT_ITEM_MODEL_H

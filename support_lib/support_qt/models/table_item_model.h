/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * This class is a simple model for table views. It allows editing and copying
 * data to the clipboard. If Copyable is set, editing is enabled, but the data
 * will not be stored, thus allowing the user to copy data to the clipboard.
 * The default implementations behaves similar to a listbox, with a table with
 * only one column. Two implement more than one column, at least getValue()
 * must be overriden.
 *
 * Note that setValue() and setRow() functions will not work unless isEditable()
 * returns true. From code this could be worked around by removing and inserting
 * a row.
 * Editable == true always implies Copyable == true as well, but for consistency,
 * Copyable should still be set.
 *
 *******************************************************************************/

#ifndef TABLE_ITEM_MODEL_H
#define TABLE_ITEM_MODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>

template <typename T>
class TableItemModel : public QAbstractTableModel
{
// Overrides
public:
	TableItemModel(void)
	{
		init();
	}

	TableItemModel(QList<QString> const &oHeader)
	{
		init();
		mHeader = oHeader;
	}

	// @Overloaded
	virtual Qt::ItemFlags flags(const QModelIndex &oIndex = QModelIndex()) const
	{
		if (!oIndex.isValid())
			return Qt::ItemIsEnabled;

		Qt::ItemFlags fl = QAbstractTableModel::flags(oIndex);
		if(isEditable(oIndex) || isCopyable(oIndex))
			fl |= Qt::ItemIsEditable;

		return fl;
	}

	// @Overloaded
	virtual int rowCount(const QModelIndex &oParent = QModelIndex()) const
	{
		return mRows.size();
	}

	// @Overloaded
	virtual int columnCount(const QModelIndex &oParent = QModelIndex()) const
	{
		return mHeader.size();
	}

	// @Overloaded
	virtual QVariant headerData(int nColumn, Qt::Orientation nOrientation, int nRole) const
	{
		if(nRole != Qt::DisplayRole)
			return QAbstractTableModel::headerData(nColumn, nOrientation, nRole);

		if(nOrientation == Qt::Vertical)
			return QAbstractTableModel::headerData(nColumn, nOrientation, nRole);

		if(nColumn < 0 || nColumn >= mHeader.size())
			return QAbstractTableModel::headerData(nColumn, nOrientation, nRole);

		return QVariant(mHeader.at(nColumn));
	}

	// @Overloaded
	virtual QVariant data(const QModelIndex &oIndex, int nRole = Qt::DisplayRole) const
	{
		if (!oIndex.isValid())
			return QVariant();

 		if(nRole != Qt::DisplayRole && nRole != Qt::EditRole)
			return QVariant();

		// Return the data to which index points.
		return getValue(oIndex);
	}

	// @Overloaded
    virtual bool setData(const QModelIndex &oIndex, const QVariant &oValue, int nRole = Qt::EditRole)
    {
		if(!oIndex.isValid())
			return false;

		if(nRole == Qt::EditRole)
		{
			if(!isEditable((oIndex)))
				return false;
		}

		return setValue(oIndex, oValue);
    }

public:
	virtual bool allowDuplicateHeader(void) const
	{
		return mAllowDuplicateHeader;
	}

	virtual void setDuplicateHeader(bool bDuplicates = true)
	{
		mAllowDuplicateHeader = bDuplicates;
	}

	virtual void setEditable(bool bEditable = true, const QModelIndex &oIndex = QModelIndex())
	{
		mEditable = bEditable;
	}

	virtual bool isEditable(const QModelIndex &oIndex = QModelIndex()) const
	{
		return mEditable;
	}

	virtual void setCopyable(bool bCopyable = true, const QModelIndex &oIndex = QModelIndex())
	{
		mCopyable = bCopyable;
	}

	/**
	 * If this is set to true, then editing is enabled, but
	 * if a value is changed it will not be stored. This can
	 * only be done if isEditable() == true.
	 */
	virtual bool isCopyable(const QModelIndex &oIndex = QModelIndex()) const
	{
		return mCopyable;
	}

	virtual bool addColumns(QList<QString> const &oHeaders)
	{
		return insertColumns(oHeaders, -1);
	}

	virtual bool insertColumns(QList<QString> const &oHeaders, int nPosition = -1)
	{
		if(!allowDuplicateHeader())
		{
			for(QString const &h : oHeaders)
			{
				if(findColumn(h) == -1)
					return false;
			}
		}

		if(nPosition < 0 || nPosition >= mHeader.size())
			nPosition = mHeader.size();

		beginInsertColumns(QModelIndex(), nPosition, nPosition+oHeaders.size());
		for(QString const &h : oHeaders)
			mHeader.insert(nPosition++, h);
		endInsertColumns();

		return true;
	}

	virtual bool addColumn(QString const &oHeader)
	{
		return insertColumn(oHeader, -1);
	}

	virtual bool insertColumn(QString const &oHeader, int nPosition = -1)
	{
		QList<QString> ql;
		ql.push_back(oHeader);
		return insertColumns(ql, nPosition);
	}

	/**
	 * Remove all columns from the table
	 */
	virtual bool removeColumns(void)
	{
		beginResetModel();
		mHeader.clear();
		endResetModel();

		return true;
	}

	virtual bool removeColumn(int nPosition)
	{
		if(nPosition >= mHeader.size())
			return false;

		beginRemoveColumns(QModelIndex(), nPosition, nPosition);
		mHeader.removeAt(nPosition);
		endRemoveColumns();

		return true;
	}

	virtual bool removeColumn(QString const &oHeader)
	{
		int i = findColumn(oHeader);
		if(i == -1)
			return false;

		beginRemoveColumns(QModelIndex(), i, i);
		mHeader.removeAt(i);
		endRemoveColumns();

		return true;
	}

	virtual int findColumn(QString const &oHeader) const
	{
		int i = 0;
		for(QString const &col : mHeader)
		{
			if(col == oHeader)
				return i;

			i++;
		}

		return -1;
	}

	virtual T const &getRow(const QModelIndex &oIndex) const
	{
		return mRows.at(oIndex.row());
	}

	virtual T getRow(const QModelIndex &oIndex)
	{
		return mRows.at(oIndex.row());
	}

	virtual T const &getRow(int nRow) const
	{
		QModelIndex i = index(nRow, 0);
		return getRow(i);
	}

	virtual T getRow(int nRow)
	{
		QModelIndex i = index(nRow, 0);
		return getRow(i);
	}

	virtual QVariant getValue(const QModelIndex &oIndex) const
	{
		QVariant v;
		if(!oIndex.isValid())
			return v;

		int row = oIndex.row();
		if(row >= mRows.size())
			return v;

		v.setValue(mRows.at(row));
		return v;
	}

	virtual QVariant getValue(int nRow, int nColumn) const
	{
		QModelIndex i = index(nRow, nColumn);
		return getValue(i);
	}

	virtual bool setValue(const QModelIndex &oIndex, const QVariant &oValue)
	{
		if(!oIndex.isValid())
			return false;

		if(!isEditable(oIndex))
			return false;

		int row = oIndex.row();
		if(row >= mRows.size())
			return false;

		mRows.replace(row, oValue.value<T>());
		emit dataChanged(oIndex, oIndex);

		return true;
	}

	/**
	 * Override setValue() to provide the data from the view to your object.
	 */
	virtual bool setValue(int nRow, int nColumn, QVariant &oValue)
	{
		QModelIndex i = index(nRow, nColumn);
		return setValue(i, oValue);
	}

	virtual bool updateRow(QModelIndex const &oIndex, const T &oRow)
	{
		if(!oIndex.isValid())
			return false;

		if(!isEditable(oIndex))
			return false;

		int row = oIndex.row();
		if(row >= mRows.size())
			return false;

		QModelIndex left = index(oIndex.row(), 0);
		QModelIndex right = index(oIndex.row(), columnCount()-1);
		mRows.replace(row, oRow);
		emit dataChanged(left, right);

		return true;
	}

	virtual bool updateRow(int nRow, const T &oRow)
	{
		QModelIndex i = index(nRow, 0);
		return updateRow(i, oRow);
	}

	virtual void addRow(T const &oItem)
	{
		insertRow(oItem);
	}

	virtual void insertRow(T const &oRow, int nRow = -1)
	{
		if(nRow < 0 || nRow > mRows.size())
			nRow = mRows.size();

		beginInsertRows(QModelIndex(), nRow, nRow);
		mRows.insert(nRow, oRow);
		endInsertRows();
	}

	virtual void removeRow(int nRow)
	{
		if(nRow >= mRows.size())
			nRow = -1;

		if(nRow < 0)
			return;

		beginRemoveRows(QModelIndex(), nRow, nRow);
		mRows.removeAt(nRow);
		endRemoveRows();
	}

	/**
	 * Remove all items from the table.
	 */
	virtual void clearRows(void)
	{
		beginRemoveRows(QModelIndex(), 0, mRows.size());
		mRows.clear();
		endRemoveRows();
	}

	/**
	 * Remove all headers from the table.
	 */
	virtual void clearColumns(void)
	{
		mHeader.clear();
	}

protected:
	virtual QList<QString> &getHeader(void)
	{
		return mHeader;
	}

	virtual QList<T> &getRows(void)
	{
		return mRows;
	}

	virtual QList<T> const &getRows(void) const
	{
		return mRows;
	}

private:
	void init(void)
	{
		mEditable = false;
		mCopyable = true;
		mAllowDuplicateHeader = true;
		clearRows();
	}

private:
	bool mAllowDuplicateHeader;
	bool mEditable;
	bool mCopyable;
	QList<QString> mHeader;
	QList<T> mRows;
};

#endif // TABLE_ITEM_MODEL_H

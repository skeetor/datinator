/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef DATATYPE_COMBOBOX_H
#define DATATYPE_COMBOBOX_H

#include <utility>
#include <vector>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QStyledItemDelegate>

#include "support/db/dbcolumn.h"

/**
 * This provides a delegate for a choice of string mappings and a type.
 * The strings are used to display the items.
 * The box will be shown in the specified rows/columns. If -1 is set for
 * rows or columns it means all rows/columns.
 */
template <typename T>
class DatatypeCombobox : public QStyledItemDelegate
{
public:
	DatatypeCombobox(int nRow, int nColumn, std::vector<std::pair<T, spt::string::string_t>> const &oTypes, QObject *oParent = 0)
			: QStyledItemDelegate(oParent)
	{
		if(nRow != -1)
		{
			std::vector<int> l;
			l.push_back(nRow);
			setRows(l);
		}
		if(nColumn != -1)
		{
			std::vector<int> l;
			l.push_back(nColumn);
			setColumns(l);
		}
		setTypeStrings(oTypes);
	}

	DatatypeCombobox(std::vector<int> const &oRows, std::vector<int> const &oColumns, std::vector<std::pair<T, spt::string::string_t>> const &oTypes, QObject *oParent = 0)
	: QStyledItemDelegate(oParent)
	{
		setRows(oRows);
		setColumns(oColumns);
		setTypeStrings(oTypes);
	}

	virtual ~DatatypeCombobox() {}

	virtual QWidget *createEditor(QWidget *oParent, const QStyleOptionViewItem &oOption, const QModelIndex &oIndex) const
	{
		if(hasEditor(oIndex))
		{
			QComboBox *cb = new QComboBox(oParent);
			cb->setAutoFillBackground(true);		// Required to show the comboboxes in the view correctly.
			for(std::pair<spt::db::DataType, spt::string::string_t> const &p : mTypeStrings)
				cb->addItem(p.second.c_str());

			return cb;
		}

		return QStyledItemDelegate::createEditor(oParent, oOption, oIndex);
	}

	virtual void setEditorData(QWidget *oEditor, const QModelIndex &oIndex) const
	{
		if(QComboBox *cb = qobject_cast<QComboBox *>(oEditor))
		{
			// get the index of the text in the combobox that matches the current value of the itenm
			QString currentText = oIndex.data(Qt::EditRole).toString();
			int cbIndex = cb->findText(currentText);

			// if it is valid, adjust the combobox
			if(cbIndex >= 0)
				cb->setCurrentIndex(cbIndex);
		}
		else
			QStyledItemDelegate::setEditorData(oEditor, oIndex);
	}

	virtual void setModelData(QWidget *oEditor, QAbstractItemModel *oModel, const QModelIndex &oIndex) const
	{
		if(QComboBox *cb = qobject_cast<QComboBox *>(oEditor))
			oModel->setData(oIndex, cb->currentText(), Qt::EditRole);
		else
			QStyledItemDelegate::setModelData(oEditor, oModel, oIndex);
	}

	void setTypeStrings(std::vector<std::pair<T, spt::string::string_t>> const &oTypeStrings)
	{
		mTypeStrings = oTypeStrings;
	}

	void setRows(std::vector<int> const &oRows)
	{
		mRows = oRows;
	}

	void setColumns(std::vector<int> const &oColumns)
	{
		mColumns = oColumns;
	}

protected:
	virtual bool hasEditor(QModelIndex const &oIndex) const
	{
		bool row = false;
		int n = oIndex.row();
		if(mRows.size() > 0)
		{
			for(int const &r : mRows)
			{
				if(r == n)
				{
					row = true;
					break;
				}
			}
		}
		else
			row = true;

		bool column = false;
		n = oIndex.column();
		if(mColumns.size() > 0)
		{
			for(int const &c : mColumns)
			{
				if(c == n)
				{
					column = true;
					break;
				}
			}
		}
		else
			column = true;

		if(row && column)
			return true;

		return false;
	}

private:
	std::vector<std::pair<T, spt::string::string_t>> mTypeStrings;
	std::vector<int> mRows;
	std::vector<int> mColumns;
};

#endif // DATATYPE_COMBOBOX_H

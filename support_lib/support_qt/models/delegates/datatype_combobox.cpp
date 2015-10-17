//******************************************************************************
//
// Datinator (c) by Gerhard W. Gruber in 2014
//
//******************************************************************************

#include <utility>
#include <QtWidgets/QComboBox>

#include "support_qt/models/delegates/datatype_combobox.h"

DatatypeCombobox::DatatypeCombobox(std::vector<int> const &oRows, std::vector<int> const &oColumns, QObject *oParent)
: QStyledItemDelegate(oParent)
{
	setRows(oRows);
	setColumns(oColumns);
}

DatatypeCombobox::DatatypeCombobox(int nRow, int nColumn, QObject *oParent)
{
	if(nRow != -1)
	{
		std::vector<int> l;
		l.append(nRow);
		setRows(l);
	}

	if(nColumn != -1)
	{
		std::vector<int> l;
		l.append(nColumn);
		setColumns(l);
	}
}

DatatypeCombobox::~DatatypeCombobox()
{
}

void DatatypeCombobox::setRows(std::vector<int> const &oRows)
{
	mRows = oRows;
}

void DatatypeCombobox::setColumns(std::vector<int> const &oColumns)
{
	mColumns = oColumns;
}

void DatatypeCombobox::setTypeStrings(std::vector<std::pair<DataType, spt::string::string_t>> const &oTypeStrings)
{
	mTypeStrings = oTypeStrings;
}

bool DatatypeCombobox::hasEditor(QModelIndex const &oIndex) const
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

QWidget *DatatypeCombobox::createEditor(QWidget *oParent, const QStyleOptionViewItem &oOption, QModelIndex const &oIndex) const
{
	if(hasEditor(oIndex))
	{
		QComboBox *cb = new QComboBox(oParent);
		cb->setAutoFillBackground(true);		// Required to show the comboboxes in the view correctly.
		for(std::pair<DataType, spt::string::string_t> const &p : mTypeStrings)
			cb->addItem(p.second);

		return cb;
	}

	return QStyledItemDelegate::createEditor(oParent, oOption, oIndex);
}

void DatatypeCombobox::setEditorData(QWidget *oEditor, const QModelIndex &oIndex) const
{
	if(QComboBox *cb = qobject_cast<QComboBox *>(oEditor))
	{
		// get the index of the text in the combobox that matches the current value of the itenm
		spt::string::string_t currentText = oIndex.data(Qt::EditRole).toString();
		int cbIndex = cb->findText(currentText);

		// if it is valid, adjust the combobox
		if(cbIndex >= 0)
			cb->setCurrentIndex(cbIndex);
	}
	else
		QStyledItemDelegate::setEditorData(oEditor, oIndex);
}

void DatatypeCombobox::setModelData(QWidget *oEditor, QAbstractItemModel *oModel, const QModelIndex &oIndex) const
{
    if(QComboBox *cb = qobject_cast<QComboBox *>(oEditor))
        oModel->setData(oIndex, cb->currentText(), Qt::EditRole);
    else
		QStyledItemDelegate::setModelData(oEditor, oModel, oIndex);
}

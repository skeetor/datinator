//******************************************************************************
//
// Datinator (c) by Gerhard W. Gruber in 2014
//
//******************************************************************************

#include <QtCore/QPair>
#include <QtWidgets/QComboBox>

#include "support/qt/models/delegates/datatype_combobox.h"

DatatypeCombobox::DatatypeCombobox(QList<int> const &oRows, QList<int> const &oColumns, QObject *oParent)
: QStyledItemDelegate(oParent)
{
	setRows(oRows);
	setColumns(oColumns);
}

DatatypeCombobox::DatatypeCombobox(int nRow, int nColumn, QObject *oParent)
{
	if(nRow != -1)
	{
		QList<int> l;
		l.append(nRow);
		setRows(l);
	}

	if(nColumn != -1)
	{
		QList<int> l;
		l.append(nColumn);
		setColumns(l);
	}
}

DatatypeCombobox::~DatatypeCombobox()
{
}

void DatatypeCombobox::setRows(QList<int> const &oRows)
{
	mRows = oRows;
}

void DatatypeCombobox::setColumns(QList<int> const &oColumns)
{
	mColumns = oColumns;
}

void DatatypeCombobox::setTypeStrings(QList<QPair<DataType, QString>> const &oTypeStrings)
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
		for(QPair<DataType, QString> const &p : mTypeStrings)
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
		QString currentText = oIndex.data(Qt::EditRole).toString();
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

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QList>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>

#include "gui/mapping/column_mapping_view_qt.moc"
#include "gui/mapping/column_mapping_delegate.h"
#include "gui/mapping/column_mapping_combobox.h"
#include "gui/mapping/column_mapping_model.h"
#include "gui/mapping/column_mapping_button_qt.moc"

#include "gui/mapping/column_mapping_item.h"
#include "support/db/dbcolumn.h"

Q_DECLARE_METATYPE(DatabaseColumn *)
Q_DECLARE_METATYPE(IManipulator *)

ColumnMappingDelegate::ColumnMappingDelegate(QList<IManipulator *> oManipulators, ColumnMappingModel *oModel, ColumnMappingView *oView, QObject *oParent)
: QStyledItemDelegate(oParent)
{
	UNUSED(oManipulators);

	mView = oView;
	mModel = oModel;
	mSourceDispatcher = NULL;
	mTargetDispatcher = NULL;
	mButtonList = new QList<ColumnMappingButton *>();
}

ColumnMappingDelegate::~ColumnMappingDelegate()
{
	for(ColumnMappingButton * const &mb : *mButtonList)
		mb->setButtonList(NULL);
}

void ColumnMappingDelegate::setDispatcher(Dispatcher<QList<DatabaseColumn *> *> *oSource, Dispatcher<QList<DatabaseColumn *> *> *oTarget)
{
	mSourceDispatcher = oSource;
	mTargetDispatcher = oTarget;
}

/**
 * The default will create an editor with the current list of columns for source and target columns.
 * When the columns are chaning, the combobox is notified via a listener and will update accordingly.
 *
 * The manipulators are loaded intot he combobox via a default list. When the user selects a manipulator
 * and configures it, a new instance is created and placed at the end of the list. If the user chooses
 * a different manipulator, the old one will be deleted.
 */
QWidget *ColumnMappingDelegate::createEditor(QWidget *oParent, const QStyleOptionViewItem &oOption, QModelIndex const &oIndex) const
{
	int c = oIndex.column();
	QWidget *editor = NULL;

	if(c == Columns::SOURCE || c == Columns::TARGET)
	{
		ColumnMappingCombobox *cb = new ColumnMappingCombobox(oParent);
		editor = cb;
		cb->setAutoFillBackground(true);
		const QList<DatabaseColumn *> *l;
		Dispatcher<QList<DatabaseColumn *> *> *dispatcher;
		if(c == Columns::SOURCE)
		{
			l = &mSourceColumns;
			dispatcher = mSourceDispatcher;
		}
		else
		{
			l = &mTargetColumns;
			dispatcher = mTargetDispatcher;
		}

		cb->setDispatcher(dispatcher);
		cb->setColumns(*l);
		cb->setCurrentIndex(oIndex.row()+1);
	}
	else if(c == Columns::MANIPULATOR)
	{
		int r = oIndex.row();
		ColumnMappingButton *b = new ColumnMappingButton(mButtonList, mModel, oParent);
		mButtonList->insert(r, b);
		b->setToolTip("Click to configure manipulators");
		editor = b;
	}

	if(!editor)
		editor = QStyledItemDelegate::createEditor(oParent, oOption, oIndex);

	return editor;
}

void ColumnMappingDelegate::setEditorData(QWidget *oEditor, const QModelIndex &oIndex) const
{
	if(oIndex.column() == Columns::MANIPULATOR)
	{
		QPushButton *b = qobject_cast<QPushButton *>(oEditor);

		ColumnMappingModel const *m = static_cast<ColumnMappingModel const *>(oIndex.model());
		ColumnMappingItem *mi = m->getRowItem(oIndex.row());
		if(mi->getManipulators().size() > 0)
		{
			QString *s = new QString();
			if(mi->getManipulators().size() > 0)
			{
				// Only the first item keeps the testvalue.
				*s = mi->getManipulators()[0]->getTestValue();
				s = mi->format(s, true);
			}

			if(s)
			{
				b->setText(*s);
				delete s;
			}
			else
				b->setText("[null]");
		}
		else
			b->setText("");
	}
	else
	{
		QComboBox *cb = qobject_cast<QComboBox *>(oEditor);
		QString currentText = oIndex.data(Qt::EditRole).toString();
  		int cbIndex = cb->findText(currentText);
		if(cbIndex == -1)
			cbIndex = 0;

		cb->setCurrentIndex(cbIndex);
	}
}

void ColumnMappingDelegate::setModelData(QWidget *oEditor, QAbstractItemModel *oModel, const QModelIndex &oIndex) const
{
	int c = oIndex.column();
//	int r = oIndex.row();
	if(c == Columns::SOURCE || c == Columns::TARGET)
	{
		QComboBox *cb = qobject_cast<QComboBox *>(oEditor);
		QVariant v = cb->currentData();
		oModel->setData(oIndex, v, Qt::EditRole);
	}
	else if(c == Columns::MANIPULATOR)
	{
		// The button will update the model, so we don't need to do anything here.
	}
    else
		QStyledItemDelegate::setModelData(oEditor, oModel, oIndex);
}

void ColumnMappingDelegate::setSourceColumns(QList<DatabaseColumn *> const &oColumns)
{
	mSourceColumns = oColumns;
}

void ColumnMappingDelegate::setTargetColumns(QList<DatabaseColumn *> const &oColumns)
{
	mTargetColumns = oColumns;
}

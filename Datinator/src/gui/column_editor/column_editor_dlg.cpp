/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "support_qt/models/delegates/datatype_combobox.h"
#include "gui/column_editor/column_editor_dlg_gui.moc"
#include "plugin/gui/datatype_string.h"

ColumnEditorDialogBox::ColumnEditorDialogBox(QWidget *oParent)
: QDialog(oParent)
{
	mGUI = new Ui::ColumnEditorDlgGUI();
	mGUI->setupUi(this);

	mModel.addRowListener(this);
	QTableView *t = mGUI->mItemTableView;
	mDatatypeDelegate = new DatatypeCombobox<supportlib::db::DataType>(-1, 1, TypeStringProvider.items());
	t->setItemDelegate(mDatatypeDelegate);
	t->verticalHeader()->setDefaultSectionSize(20);
	t->setModel(&mModel);
}

ColumnEditorDialogBox::~ColumnEditorDialogBox(void)
{
	delete mDatatypeDelegate;
	freeColumns();
}

void ColumnEditorDialogBox::accept(void)
{
	QDialog::accept();
}

void ColumnEditorDialogBox::reject(void)
{
	QDialog::reject();
}

void ColumnEditorDialogBox::onReset(void)
{
	mModel.clear();
	QList<DatabaseColumn *>l = mOriginal;
	setColumns(l);
}

void ColumnEditorDialogBox::setColumns(QList<DatabaseColumn *> const &oColumns)
{
	freeColumns();
	setResult(QDialog::Rejected);
	mOriginal = oColumns;
	QList<DatabaseColumn *>l ;
	for(DatabaseColumn * const &col : oColumns)
		l.append(col->duplicate());
	mModel.setColumns(l);
}

QList<DatabaseColumn *> ColumnEditorDialogBox::getColumns(void)
{
	int n = mModel.rowCount()-1;
	QList<DatabaseColumn *>l;
	for(int r = 0; r < n; r++)
	{
		DatabaseColumn *col = mModel.columnItem(r);
		if(col)
			l.append(col);
	}

	return l;
}

void ColumnEditorDialogBox::handleNotification(Dispatcher<bool, int, int> *oDispatcher, bool bInserted, int nRow, int nCount)
{
	UNUSED(oDispatcher);

	if(bInserted)
	{
		QTableView *v = mGUI->mItemTableView;
		for(int r = nRow; r < nRow+nCount; r++)
		{
			QModelIndex index = mModel.index(r, 1);
			v->openPersistentEditor(index);
		}
	}
}

void ColumnEditorDialogBox::freeColumns(void)
{
	int n = mModel.rowCount()-1;
	for(int r = 0; r < n; r++)
	{
		DatabaseColumn *col = mModel.columnItem(r);
		if(col)
			delete col;
	}
	mModel.clear();
}

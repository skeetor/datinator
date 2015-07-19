/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>

#include "plugin/gui/datatype_string.h"
#include "plugin/gui/sample_table/sample_table_view.h"
#include "support_qt/models/delegates/datatype_combobox.h"

SampleTableView::SampleTableView(QWidget *oParent)
: QTableView(oParent)
{
	verticalHeader()->setDefaultSectionSize(20);
	mDatatypeBox = NULL;

	mShowDatatypes = false;
	mItemModel.showDatatypes(false);
	setModel(&mItemModel);
}

void SampleTableView::enableDatatypes(bool bEnableDatatypes)
{
	mShowDatatypes = false;
	if(bEnableDatatypes)
	{
		mShowDatatypes = true;
		if(!mDatatypeBox)
		{
			mItemModel.showDatatypes(true);
			mDatatypeBox = new DatatypeCombobox<supportlib::db::DataType>(5, -1, TypeStringProvider.items(), this);
			setItemDelegate(mDatatypeBox);
			clearRows();
		}
	}
	else
	{
		mItemModel.showDatatypes(false);
		if(mDatatypeBox)
			delete mDatatypeBox;

		mDatatypeBox = NULL;
	}
}

SampleTableView::~SampleTableView(void)
{
	if(mDatatypeBox)
		delete mDatatypeBox;
}

void SampleTableView::setContainer(ContainerBase *oContainer)
{
	mContainer = oContainer;
	if(mDatatypeBox)
		mDatatypeBox->setTypeStrings(TypeStringProvider.items());
}

void SampleTableView::updateWidgetView(void)
{
	if(!mShowDatatypes)
		return;

	QAbstractItemModel *m = model();
	int n = m->columnCount();
	int r = m->rowCount()-1;

	for(int i = 0; i < n; i++)
	{
		QModelIndex index = m->index(r, i);
		openPersistentEditor(index);
	}
}

void SampleTableView::setColumns(QList<DatabaseColumn *> const &oColumns)
{
	mItemModel.setColumns(oColumns);
}

void SampleTableView::setRows(QList<QList<QString>> const &oSampleRows)
{
	mItemModel.setRows(oSampleRows);
	updateWidgetView();
}

void SampleTableView::clearRows(void)
{
	mItemModel.clearRows();
}

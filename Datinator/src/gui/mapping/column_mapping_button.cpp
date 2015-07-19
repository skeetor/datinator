/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "gui/mapping/column_mapping_button_qt.moc"
#include "gui/mapping/column_mapping_model.h"
#include "manipulator/editor/manipulator_editor_dlg_gui.moc"

ColumnMappingButton::ColumnMappingButton(QList<ColumnMappingButton *> *oButtonList, ColumnMappingModel *oModel, QWidget *oParent)
	: QPushButton(oParent)
{
	mModel = oModel;
	mButtonList = oButtonList;

	connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

ColumnMappingButton::~ColumnMappingButton(void)
{
	if(mButtonList)
		mButtonList->removeAll(this);
}

void ColumnMappingButton::setButtonList(QList<ColumnMappingButton *> *oButtonList)
{
	mButtonList = oButtonList;
}

void ColumnMappingButton::onClicked()
{
	int row = mButtonList->indexOf(this);
	QList<IManipulator *>items = mModel->getManipulators(row);

	QList<DatabaseColumn *>l;
	int n = mModel->rowCount();
	for(int i = 0; i < n; i++)
		l.append(mModel->getSourceColumn(i));

	ManipulatorEditorDialogBox dlg(items, l, this);
	dlg.exec();
	if(dlg.result() == QDialog::Accepted)
	{
		QList<IManipulator *> ml = dlg.getManipulators();
		mModel->setManipulators(row, ml);
	}
}

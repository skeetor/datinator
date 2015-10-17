/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "gui/mapping/column_mapping_button_qt.moc"
#include "gui/mapping/column_mapping_model.h"
#include "manipulator/editor/manipulator_editor_dlg_gui.moc"

ColumnMappingButton::ColumnMappingButton(std::vector<ColumnMappingButton *> *oButtonList, ColumnMappingModel *oModel, QWidget *oParent)
	: QPushButton(oParent)
{
	mModel = oModel;
	mButtonList = oButtonList;

	connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

ColumnMappingButton::~ColumnMappingButton(void)
{
	if(mButtonList)
		mButtonList->erase(std::remove(mButtonList->begin(), mButtonList->end(), this), mButtonList->end());
}

void ColumnMappingButton::setButtonList(std::vector<ColumnMappingButton *> *oButtonList)
{
	mButtonList = oButtonList;
}

void ColumnMappingButton::onClicked()
{
	int row = std::find(mButtonList->begin(), mButtonList->end(), this) - mButtonList->begin();
	std::vector<IManipulator *>items = mModel->getManipulators(row);

	std::vector<DatabaseColumn *>l;
	int n = mModel->rowCount();
	for(int i = 0; i < n; i++)
		l.push_back(mModel->getSourceColumn(i));

	ManipulatorEditorDialogBox dlg(items, l, this);
	dlg.exec();
	if(dlg.result() == QDialog::Accepted)
	{
		std::vector<IManipulator *> ml = dlg.getManipulators();
		mModel->setManipulators(row, ml);
	}
}

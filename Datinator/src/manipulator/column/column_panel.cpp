/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <QtGui/QRegExpValidator>

#include "manipulator/column/column_panel_gui.moc"
#include "manipulator/column/column.h"
#include "support/unicode/unicode_types.h"
#include "support/db/dbcolumn.h"

#include "support/helper/string.h"

ColumnManipulatorPanel::ColumnManipulatorPanel(ColumnManipulator *oOwner, QWidget *oParent)
	: ManipulatorPanel(oOwner, oParent)
{
	mGUI = new Ui::ColumnManipulatorGUI();
	mGUI->setupUi(this);
	setButtons(mGUI->mPrependBtn, mGUI->mAppendBtn, mGUI->mReplaceBtn);
	std::vector<DatabaseColumn *> cols = oOwner->getSourceColumns();
	for(DatabaseColumn * const &col : cols)
	{
		if(col)
			mGUI->mColumnBox->addItem(spt::string::toQt(col->getName()));
	}

	onColumnSelected(0);
}

ColumnManipulatorPanel::~ColumnManipulatorPanel(void)
{
}

void ColumnManipulatorPanel::onColumnSelected(int nColumn)
{
	UNUSED(nColumn);

	notifyOwner();
}

void ColumnManipulatorPanel::setColumn(ssize_t nColumnIndex)
{
	mGUI->mColumnBox->setCurrentIndex(nColumnIndex);
}

ssize_t ColumnManipulatorPanel::getColumn(void)
{
	return mGUI->mColumnBox->currentIndex();
}

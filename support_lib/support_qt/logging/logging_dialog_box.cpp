/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "support_qt/logging/logging_dialog_box_gui.moc"
#include "support_qt/logging/logging_item_model.h"

LoggingDialogBox::LoggingDialogBox(QWidget *oParent)
: QDialog(oParent)
{
	mOK = false;

	mGUI = new Ui::LoggingDialogGUI();
	mGUI->setupUi(this);

	mModel = new LoggingItemModel(this);
	QTableView *t = mGUI->mItemTableView;
	t->setSelectionMode(QAbstractItemView::SingleSelection);
	t->verticalHeader()->setVisible(false);
	t->verticalHeader()->setDefaultSectionSize(20);
	t->horizontalHeader()->setVisible(true);
	t->horizontalHeader()->setStretchLastSection(true);
	t->setModel(mModel);
}

LoggingDialogBox::~LoggingDialogBox(void)
{
}

void LoggingDialogBox::setLog(supportlib::logging::Logging const &oLog)
{
	mModel->setItems(oLog.getItems());
}

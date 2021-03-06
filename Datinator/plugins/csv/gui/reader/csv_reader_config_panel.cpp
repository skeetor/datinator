/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QVariant>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include "csv/csv_reader.h"
#include "csv/gui/reader/csv_reader_config_panel_gui.moc"

CSVReaderConfigPanel::CSVReaderConfigPanel(QWidget *oParent)
	: QFrame(oParent)
{
	mGUI = new Ui::CSVReaderConfigPanelGUI();
	mGUI->setupUi(this);
	mGUI->mCSVTableView->enableDatatypes();
}

CSVReaderConfigPanel::~CSVReaderConfigPanel(void)
{
}

void CSVReaderConfigPanel::setColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	mGUI->mCSVTableView->setColumns(oColumns);
}

void CSVReaderConfigPanel::setRows(std::vector<std::vector<StdString>> const &oRows)
{
	mGUI->mCSVTableView->setRows(oRows);
}

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include "csv/csv_reader.h"
#include "csv/gui/reader/csv_reader_config_panel_gui.moc"

CSVReaderConfigPanel::CSVReaderConfigPanel(CSVReader *oContainer, QWidget *oParent)
:	FilePanelConfigPanel(oParent)
{
	mGUI = new Ui::CSVReaderConfigPanelGUI();
	mGUI->setupUi(this);
	mGUI->mCSVTableView->enableDatatypes();
	mGUI->mCSVTableView->setContainer(oContainer);
}

CSVReaderConfigPanel::~CSVReaderConfigPanel(void)
{
}

void CSVReaderConfigPanel::setColumns(QList<DatabaseColumn *> const &oColumns)
{
	mGUI->mCSVTableView->setColumns(oColumns);
}

void CSVReaderConfigPanel::setRows(QList<QList<QString>> const &oRows)
{
	mGUI->mCSVTableView->setRows(oRows);
}

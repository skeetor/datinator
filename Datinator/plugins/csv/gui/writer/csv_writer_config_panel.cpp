/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include "csv/csv_writer.h"
#include "csv/gui/writer/csv_writer_config_panel_gui.moc"

CSVWriterConfigPanel::CSVWriterConfigPanel(CSVWriter *oContainer, QWidget *oParent)
:	FilePanelConfigPanel(oParent)
{
	UNUSED(oContainer);

	mGUI = new Ui::CSVWriterConfigPanelGUI();
	mGUI->setupUi(this);
}

CSVWriterConfigPanel::~CSVWriterConfigPanel(void)
{
	delete mGUI;
}

void CSVWriterConfigPanel::reset(void)
{
	mGUI->mSeparatorSemicolonBtn->setChecked(true);
}

StdChar CSVWriterConfigPanel::getSeparator(void)
{
	if(mGUI->mSeparatorSemicolonBtn->isChecked())
		return ';';

	if(mGUI->mSeparatorColonBtn->isChecked())
		return ':';

	if(mGUI->mSeparatorSpaceBtn->isChecked())
		return ' ';

	if(mGUI->mSeparatorTabBtn->isChecked())
		return '\t';

	if(mGUI->mSeparatorCommaBtn->isChecked())
		return ',';

	if(mGUI->mSeparatorCustomBtn->isChecked())
	{
		QString s = mGUI->mSeparatorCustomTxt->text();
		if(s.length() > 0)
		{
			StdString ns = supportlib::string::QtStringToStringT(s);
			return ns[0];
		}
	}

	return ';';
}

void CSVWriterConfigPanel::setSeparator(StdChar oSeparator)
{
	if(oSeparator == ';')
	{
		mGUI->mSeparatorSemicolonBtn->setChecked(true);
		return ;
	}

	if(oSeparator == ':')
	{
		mGUI->mSeparatorColonBtn->setChecked(true);
		return ;
	}

	if(oSeparator == ' ')
	{
		mGUI->mSeparatorSpaceBtn->setChecked(true);
		return ;
	}

	if(oSeparator == '\t')
	{
		mGUI->mSeparatorTabBtn->setChecked(true);
		return ;
	}

	if(oSeparator == ',')
	{
		mGUI->mSeparatorCommaBtn->setChecked(true);
		return ;
	}

	mGUI->mSeparatorCustomBtn->setChecked(true);
	mGUI->mSeparatorCustomTxt->setText(QString(oSeparator));
}

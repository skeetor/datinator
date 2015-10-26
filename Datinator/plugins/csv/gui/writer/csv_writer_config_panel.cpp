/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include <support/helper/string.h>

#include "csv/csv_writer.h"
#include "csv/gui/writer/csv_writer_config_panel_gui.moc"

CSVWriterConfigPanel::CSVWriterConfigPanel(QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::CSVWriterConfigPanelGUI();
	mGUI->setupUi(this);
}

CSVWriterConfigPanel::~CSVWriterConfigPanel(void)
{
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
		StdString s = spt::string::fromQt(mGUI->mSeparatorCustomTxt->text());
		if(s.length() > 0)
			return s[0];
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
	StdString s;
	s = oSeparator;
	mGUI->mSeparatorCustomTxt->setText(spt::string::toQt(s));
}

StdChar CSVWriterConfigPanel::getBracket(StdChar &oClose) const
{
	oClose = 0;

	if(mGUI->mQuoteBtn->isChecked())
	{
		oClose = '"';
		return '"';
	}

	if(mGUI->mParenthesisBtn->isChecked())
	{
		oClose = ')';
		return '(';
	}

	if(mGUI->mBracketBtn->isChecked())
	{
		oClose = ']';
		return '[';
	}

	if(mGUI->mCurlyBtn->isChecked())
	{
		oClose = '}';
		return '{';
	}

	if(mGUI->mSignBtn->isChecked())
	{
		oClose = '>';
		return '<';
	}

	if(mGUI->mCustomQuoteBtn->isChecked())
	{
		StdString s = spt::string::fromQt(mGUI->mSeparatorCustomTxt->text());
		if(s.length() > 0)
		{
			if(s.length() > 1)
				oClose = s[1];
			else
			{
				switch(s[0])
				{
					case '\'':
					case '"':
						oClose = s[0];
					break;
				}
			}

			return s[0];
		}
	}

	return 0;
}

void CSVWriterConfigPanel::setBracket(StdChar oOpen, StdChar oClose)
{
	if(oOpen == '"' && oClose == 0)
	{
		mGUI->mQuoteBtn->setChecked(true);
		return;
	}

	if(oOpen == '(' && oClose == ')')
	{
		mGUI->mParenthesisBtn->setChecked(true);
		return;
	}

	if(oOpen == '[' && oClose == ']')
	{
		mGUI->mBracketBtn->setChecked(true);
		return;
	}

	if(oOpen == '{' && oClose == '}')
	{
		mGUI->mCurlyBtn->setChecked(true);
		return;
	}

	if(oOpen == '<'  && oClose == '>')
	{
		mGUI->mSignBtn->setChecked(true);
		return;
	}

	mGUI->mCustomQuoteBtn->setChecked(true);
	QString s;
	s += oOpen;
	s += oClose;

	mGUI->mCustomQuoteTxt->setText(s);
}

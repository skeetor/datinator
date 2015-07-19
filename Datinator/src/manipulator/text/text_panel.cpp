/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <QtGui/QRegExpValidator>

#include "manipulator/text/text_panel_gui.moc"
#include "manipulator/text/text.h"


// First character has to start with a dash or 1-9. After followsPOSITION_LIST_REG_EX
// either a comma or another number.
// Alternativly the first character is 1-9 followed by an arbitrary digit
// or a dash followed by a comma
// Repeat as often as needed.
#define POSITION_LIST_REG_EX "((([1-9]|-)[0-9]*,)|([1-9][[0-9]*-[0-9]*,))*"

QRegExp gPositionListRegEx(POSITION_LIST_REG_EX);

const char *gFixedHelp = "<b><big>Fixed Text</big></b><br><br>"
	"Fixed text replaces the current value with the new one. The new value may also be empty.";
const char *gLimitHelp = "<b><big>Set the minimum and maximum length</big></b><br><br>"
	"This ensures that the text doesn't exceed the specified limits.<br>"
	"If either <b>MinLength</b> or <b>MaxLength</b> is 0, the corresponding size is ignored.<br>"
	"If <b>MinLength</b> is > 0 and the text is shorter it will be padded.<br>"
	"The padding is done with the fill character (default is 'BLANK') and can either be at the start or the end of the text.<br>."
	"If <b>MaxLength</b> is > 0 and the text is longer it will be cut.<br>."
	"<br>"
	;
const char *gReplaceRegExHelp = "<b><big>Replace by text or regular expression</big></b><br><br>";
const char *gSelectRegExHelp = "<b><big>Select by regular expression/wildcard</big></b><br><br>"
	"Select (part of) the string by using a regular expression or a wildcard pattern.<br>"
	"Using wildcards is the same as in a command shell.<br>"
	"<br>"
	;
const char *gSelectPositionHelp = "<b><big>Select by position</big></b><br><br>"
	"You can enter a list of numbers which characters should be taken.<br>"
	"Example: '5,4,3-8,-5,7-'<br><br>"
	"A range is identified by a '-'. If one part of a range is not specified it means either<br>"
	"from the start or until the end of the string. Each number(s) are separated by a ','.<br>"
	"The count starts at 1, so 1 means the first character of the string.<br>"
	"<br>"
	;
const char *gToUpperHelp = "<b><big>Convert to upper case</big></b><br><br>";
const char *gToLowerHelp = "<b><big>Convert to lower case</big></b><br><br>";
const char *gMultilineHelp = "<b><big>Convert from multiline string to single line or vice versa</big></b><br><br>"
	"If a database column contains strings with free text, which can include i.E. linebreaks or quotes "
	"it would cause an invalid file if exported to CSV. In such a case these characters have to be<br>"
	"masked to create an entry with a single line without linebreaks.<br><br>"
	"The default is to convert from multine to single line. If \"Invert Selection\" is selected the "
	"masked characters will be restored. For example when importing such a CSV back into a database.<br>"
;

const char *gHelpTexts[SEL_MAX] =
{
	gFixedHelp,
	gLimitHelp,
	gReplaceRegExHelp,
	gSelectRegExHelp,
	gSelectPositionHelp,
	gToUpperHelp,
	gToLowerHelp,
	gMultilineHelp
};

TextManipulatorPanel::TextManipulatorPanel(TextManipulator *oOwner, QWidget *oParent)
	: ManipulatorPanel(oOwner, oParent)
{
	mGUI = new Ui::TextManipulatorGUI();
	mGUI->setupUi(this);
	setButtons(mGUI->mPrependBtn, mGUI->mAppendBtn, mGUI->mReplaceBtn);

	QPalette editable = mGUI->mHelpTxt->palette();
	QColor col = editable.color(QPalette::Button);
	editable.setColor(QPalette::Base, col);
	editable.setColor(QPalette::Text, Qt::black);
	mGUI->mHelpTxt->setPalette(editable);
	mGUI->mHelpTxt->setReadOnly(true);

	mGUI->mPositionTxt->setValidator(new QRegExpValidator(gPositionListRegEx, this));

	mGUI->mSelectionBox->addItem("Fixed Text");
	mGUI->mSelectionBox->addItem("Limit");
	mGUI->mSelectionBox->addItem("Replace by text or regular expression");
	mGUI->mSelectionBox->addItem("Select by regular expression");
	mGUI->mSelectionBox->addItem("Select by position");
	mGUI->mSelectionBox->addItem("Convert to upper case");
	mGUI->mSelectionBox->addItem("Convert to lower case");
	mGUI->mSelectionBox->addItem("Convert multiline/single line");
	onAction(0);
}

TextManipulatorPanel::~TextManipulatorPanel(void)
{
}

QString TextManipulatorPanel::getText(void) const
{
	return mGUI->mTextTxt->text();
}

void TextManipulatorPanel::setText(QString const &oText)
{
	mGUI->mTextTxt->setText(oText);
}

void TextManipulatorPanel::setAction(TextManipulatorAction nAction)
{
	mGUI->mSelectionBox->setCurrentIndex(nAction);
	onAction(nAction);
}

TextManipulatorAction TextManipulatorPanel::getAction(void) const
{
	return static_cast<TextManipulatorAction>(mGUI->mSelectionBox->currentIndex());
}

void TextManipulatorPanel::setLength(int nMinLength, int nMaxLength, int nFillCharacter, bool bFillAtEnd)
{
	QString fc;
	if(nFillCharacter)
		fc += nFillCharacter;

	mGUI->mFillCharacterTxt->setText(fc);
	mGUI->mMinLengthSpin->setValue(nMinLength);
	mGUI->mMaxLengthSpin->setValue(nMaxLength);
	mGUI->mFillAtEndBox->setChecked(bFillAtEnd);
}

int TextManipulatorPanel::getMinLength(void) const
{
	return mGUI->mMinLengthSpin->value();
}

int TextManipulatorPanel::getMaxLength(void) const
{
	return mGUI->mMaxLengthSpin->value();
}

int TextManipulatorPanel::getFillCharacter(void) const
{
	QString s = mGUI->mFillCharacterTxt->text();
	if(s.length() > 0)
		return s.at(0).unicode();

	return 0;
}

bool TextManipulatorPanel::getFillAtEnd(void) const
{
	return mGUI->mFillAtEndBox->isChecked();
}

bool TextManipulatorPanel::setPositions(QList<QPair<int, int>> const &oPositionList)
{
	UNUSED(oPositionList);

	return false;
}

bool TextManipulatorPanel::getPositions(QList<QPair<int, int>> &oPositionList) const
{
	return toPositionList(getPositionText(), oPositionList);
}

bool TextManipulatorPanel::setPositionText(QString const &oPositionString)
{
	if(oPositionString.length() > 0)
	{
		QList<QPair<int, int>>l;
		if(!toPositionList(oPositionString, l))
			return false;
	}

	mGUI->mPositionTxt->setText(oPositionString);
	return true;
}

QString TextManipulatorPanel::getPositionText(void) const
{
	return mGUI->mPositionTxt->text();
}

bool toPositionText(QList<QPair<int, int>> const &oPositionList, QString &oPositionText)
{
	oPositionText = "";

	if(oPositionList.size() == 0)
		return true;

	for(QPair<int, int> const &p : oPositionList)
	{
		if(p.first < 1 && p.second < 1)
			return false;

		QPair<int, int> pos = p;
		if(pos.first == pos.second)
			pos.second = 0;

		if(pos.first > 0)
			oPositionText += QString::number(pos.first);

		if(pos.second != 0)
		{
			oPositionText += '-';
			if(pos.second != -1)
				oPositionText += QString::number(pos.second);
		}
	}

	return true;
}

bool toPositionList(QString const &oPositionString, QList<QPair<int, int>> &oPositionList)
{
	oPositionList.clear();

//	if(oPositionString.length() > 0 && !gPositionListRegEx.exactMatch(oPositionString))
//		return false;

	if(oPositionString.length() == 0)
		return true;

	QString s = oPositionString;
	if(s[s.length()-1] != ',')
		s += ',';

	QPair<int, int> p;
	bool range = false;
	int n = 0;
	int chars = 0;			// length of the number
	p.first = 0;
	p.second = 0;
    for(int i = 0; i < s.length(); i++)
	{
		int c = s.at(i).toLatin1();
		if(c == '-')
		{
			if(range)
				return false;
			else
				range = true;

			p.first = n;
			p.second = -1;
			n = 0;
			chars = 0;
			continue;
		}

		if(c == ',')
		{
			if(range)
			{
				if(chars == 0)
					n = -1;

				p.second = n;
			}
			else
				p.first = n;

			if(p.first < 1 && p.second < 1)
				return false;

			if(p.first == p.second)
				p.second = 0;

			// If we have a range and the end position is lower
			// then we swap it.
			if(p.first > p.second && p.second > 0)
			{
				int x = p.first;
				p.first = p.second;
				p.second = x;
			}

			oPositionList.append(p);

			n = 0;
			p.first = 0;
			p.second = 0;
			chars = 0;
			range = false;
			continue;
		}

		n = (n*10)+(c-'0');
		chars++;
	}

	return true;
}

void TextManipulatorPanel::setPattern(QString const &oPattern, bool bInvertSelection, bool bRegularExpression)
{
	mGUI->mPatternTxt->setText(oPattern);
	mGUI->mRegularExpressionBox->setChecked(bRegularExpression);
	mGUI->mInvertBox->setChecked(bInvertSelection);
}

bool TextManipulatorPanel::getInvertSelection(void) const
{
	return mGUI->mInvertBox->isChecked();
}

bool TextManipulatorPanel::getRegularExpression(void) const
{
	return mGUI->mRegularExpressionBox->isChecked();
}

QString TextManipulatorPanel::getPattern(void) const
{
	return mGUI->mPatternTxt->text();
}

void TextManipulatorPanel::onTextChanged(QString oText)
{
	UNUSED(oText);

	notifyOwner();
}

void TextManipulatorPanel::onValueChanged(int nValue)
{
	UNUSED(nValue);

	switch(getAction())
	{
		case TextManipulatorAction::SEL_Limit:
		{
			int min = mGUI->mMinLengthSpin->value();
			int max = mGUI->mMaxLengthSpin->value();

			if(max && min > max)
			{
				mGUI->mMaxLengthSpin->setValue(min);
				return;
			}
		}
		break;

		default:
		break;
	}

	notifyOwner();
}

void TextManipulatorPanel::onAction(int nSelectionIndex)
{
	disableAll();

	mGUI->mHelpTxt->setHtml(QApplication::translate("TextManipulatorGUI", gHelpTexts[nSelectionIndex]));

	switch(nSelectionIndex)
	{
		case TextManipulatorAction::SEL_Text:
		{
			mGUI->mTextTxt->setEnabled(true);
		}
		break;

		case TextManipulatorAction::SEL_Limit:
		{
			mGUI->mFillCharacterTxt->setEnabled(true);
			mGUI->mFillAtEndBox->setEnabled(true);
			mGUI->mMinLengthSpin->setEnabled(true);
			mGUI->mMaxLengthSpin->setEnabled(true);
		}
		break;

		case TextManipulatorAction::SEL_ReplacePattern:
		{
			mGUI->mTextTxt->setEnabled(true);
			mGUI->mRegularExpressionBox->setEnabled(true);
			mGUI->mPatternTxt->setEnabled(true);
		}
		break;

		case TextManipulatorAction::SEL_SelectPattern:
		{
			mGUI->mRegularExpressionBox->setEnabled(true);
			mGUI->mPatternTxt->setEnabled(true);
			mGUI->mInvertBox->setEnabled(true);
		}
		break;

		case TextManipulatorAction::SEL_SelectPosition:
		{
			mGUI->mPositionTxt->setEnabled(true);
		}
		break;

		case TextManipulatorAction::SEL_ToLower:
		case TextManipulatorAction::SEL_ToUpper:
		break;

		case TextManipulatorAction::SEL_Multiline:
		{
			mGUI->mInvertBox->setEnabled(true);
		}
		break;
	}

	notifyOwner();
}

void TextManipulatorPanel::disableAll(void)
{
    mGUI->mTextTxt->setDisabled(true);

    mGUI->mPositionTxt->setDisabled(true);

    mGUI->mFillCharacterTxt->setDisabled(true);
    mGUI->mMinLengthSpin->setDisabled(true);
    mGUI->mMaxLengthSpin->setDisabled(true);
    mGUI->mFillAtEndBox->setDisabled(true);

    mGUI->mRegularExpressionBox->setDisabled(true);
	mGUI->mInvertBox->setDisabled(true);
    mGUI->mPatternTxt->setDisabled(true);
}

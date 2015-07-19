/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QSettings>

#include <support/helper/string.h>
#include <support/unicode/unicode_types.h>

#include "manipulator/text/text.h"

TextManipulator::TextManipulator(void)
{
	mPanel = NULL;
	reset();
}

TextManipulator::TextManipulator(TextManipulator const &oSource)
{
	mPanel = NULL;
	reset();
	copy(oSource);
}

TextManipulator::~TextManipulator(void)
{
}

bool TextManipulator::isConfigured(void)
{
	readValues();

	switch(getAction())
	{
		case TextManipulatorAction::SEL_Text:
		{
		}
		break;

		case TextManipulatorAction::SEL_Limit:
		{
			int min_val = getMinLength();
			int max_val = getMaxLength();
			if(min_val > max_val)
			{
				setError("The minimum length may not be higher than the maximum length!");
				return false;
			}
		}
		break;

		case TextManipulatorAction::SEL_SelectPattern:
		case TextManipulatorAction::SEL_ReplacePattern:
		{
			if(getPattern().length() == 0)
			{
				setError("A pattern or a regular expression must be specified");
				return false;
			}
		}
		break;

		case TextManipulatorAction::SEL_SelectPosition:
		{
			// Valid input is ensured by the regular exression.
		}
		break;

		case TextManipulatorAction::SEL_ToLower:
		case TextManipulatorAction::SEL_ToUpper:
		case TextManipulatorAction::SEL_Multiline:
		{
			// Nothing to do;
		}
		break;

		default:
		{
			setError("Internal Error! Invalid Action value:"+ QString::number(getAction()));
			return false;
		}
	}

	return true;
}

QString TextManipulator::getId(void)
{
	return "DBEF3EA0-FFC0-11E3-9191-0800200C9A66";
}

void TextManipulator::reset(void)
{
	Manipulator::reset();
	setAction(TextManipulatorAction::SEL_Text);
	setText("");
	setLength(0, 0, 0, true);
	setPositions(QList<QPair<int, int>>());
	setPattern("", false, false);
	setValues();
}

void TextManipulator::setText(QString const &oText)
{
	mText = oText;
}

QString TextManipulator::getText(void) const
{
	return mText;
}

void TextManipulator::setAction(TextManipulatorAction nAction)
{
	mAction = nAction;
}

TextManipulatorAction TextManipulator::getAction(void) const
{
	return mAction;
}

void TextManipulator::setLength(int nMinLength, int nMaxLength, int nFillCharacter, bool bFillAtEnd)
{
	mMinLength = nMinLength;
	mMaxLength = nMaxLength;
	mFillCharacter = nFillCharacter;
	mFillAtEnd = bFillAtEnd;
}

int TextManipulator::getMinLength(void) const
{
	return mMinLength;
}

int TextManipulator::getMaxLength(void) const
{
	return mMaxLength;
}

int TextManipulator::getFillCharacter(void) const
{
	return mFillCharacter;
}

bool TextManipulator::getFillAtEnd(void) const
{
	return mFillAtEnd;
}

bool TextManipulator::setPositions(QList<QPair<int, int>> const &oPositionList)
{
	mPositions = oPositionList;
	return true;
}

QList<QPair<int, int>> TextManipulator::getPositions(void) const
{
	return mPositions;
}

void TextManipulator::setPattern(QString const &oPattern, bool bInvertSelection, bool bRegularExpression)
{
	mPattern = oPattern;
	mRegularExpression = bRegularExpression;
	mInvertSelection = bInvertSelection;
}

bool TextManipulator::getInvertSelection(void) const
{
	return mInvertSelection;
}

bool TextManipulator::getRegularExpression(void) const
{
	return mRegularExpression;
}

QString TextManipulator::getPattern(void) const
{
	return mPattern;
}

TextManipulator &TextManipulator::operator=(TextManipulator const &oSource)
{
	reset();
	copy(oSource);
	return *this;
}

void TextManipulator::copy(TextManipulator const &oSource)
{
	reset();
	Manipulator::copy(oSource);
	setAction(oSource.getAction());
	setLength(oSource.getMinLength(), oSource.getMaxLength(), oSource.getFillCharacter(), oSource.getFillAtEnd());
	setPositions(oSource.getPositions());
	setPattern(oSource.getPattern(), oSource.getInvertSelection(), oSource.getRegularExpression());
	setText(oSource.getText());
	setValues();
}

QString TextManipulator::getName(void)
{
	return "String formatting";
}

QString TextManipulator::getDescription(void)
{
	return "Returns a text value";
}

IManipulator *TextManipulator::createInstance(void)
{
	return new TextManipulator();
}

IManipulator *TextManipulator::duplicate(void)
{
	return new TextManipulator(*this);
}

void TextManipulator::readValues(void)
{
	if(!mPanel)
		return;

	setMode(mPanel->getMode());

	setAction(mPanel->getAction());
	setLength(mPanel->getMinLength(), mPanel->getMaxLength(), mPanel->getFillCharacter(), mPanel->getFillAtEnd());
	QList<QPair<int, int>>l;
	mPanel->getPositions(l);
	setPositions(l);
	setPattern(mPanel->getPattern(), mPanel->getInvertSelection(), mPanel->getRegularExpression());
	setText(mPanel->getText());
}

void TextManipulator::setValues(void)
{
	if(!mPanel)
		return;

	mPanel->setMode(getMode());

	mPanel->setAction(getAction());
	mPanel->setLength(getMinLength(), getMaxLength(), getFillCharacter(), getFillAtEnd());
	mPanel->setPositions(getPositions());
	mPanel->setPattern(getPattern(), getInvertSelection(), getRegularExpression());
	mPanel->setText(getText());
}

QWidget *TextManipulator::getConfigurationPanel(QWidget *oParent)
{
	if(mPanel == NULL)
		mPanel = new TextManipulatorPanel(this, oParent);

	setValues();

	return mPanel;
}

QString *TextManipulator::format(QString *oValue, bool bPreview)
{
	UNUSED(bPreview);

	QString s;
	QString v;

	if(oValue)
		v = *oValue;

	switch(getAction())
	{
		case TextManipulatorAction::SEL_Limit:
		{
			s = v;
			int n = s.length();
			if(mMaxLength)
			{
				if(n > mMaxLength)
					s = s.mid(0, mMaxLength);
			}

			bool end = getFillAtEnd();
			int c = getFillCharacter();
			if(!c)
				c = ' ';

			while(s.length() < mMinLength)
			{
				if(end)
					s += c;
				else
					s = c + s;
			}
		}
		break;

		case TextManipulatorAction::SEL_SelectPattern:
			s = selectByPattern(v, getPattern(), getRegularExpression(), getInvertSelection(), NULL);
		break;

		case TextManipulatorAction::SEL_ReplacePattern:
		{
			QString replace = getText();
			s = selectByPattern(v, getPattern(), getRegularExpression(), getInvertSelection(), &replace);
		}
		break;

		case TextManipulatorAction::SEL_SelectPosition:
			s = selectByPosition(v);
		break;

		case TextManipulatorAction::SEL_ToLower:
		{
			if(oValue)
			{
				s = oValue->toLower();
				delete oValue;
				oValue = NULL;
			}
		}
		break;

		case TextManipulatorAction::SEL_ToUpper:
		{
			if(oValue)
			{
				s = oValue->toUpper();
				delete oValue;
				oValue = NULL;
			}
		}
		break;

		case TextManipulatorAction::SEL_Multiline:
		{
			if(!oValue)
				break;

			StdString line = supportlib::string::QtStringToStringT(*oValue);
			if(getInvertSelection())
				line = supportlib::string::unmaskString(line);
			else
				line = supportlib::string::maskString(line);
			s = supportlib::string::StringTToQtString(line);
		}
		break;

		case TextManipulatorAction::SEL_Text:
		default:
			s = getText();
		break;
	}

	return applyMode(oValue, s);
}

bool TextManipulator::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!Manipulator::loadProfile(oProfile, oKey))
		return false;

	setText(oProfile.value(oKey+"_text", "").toString());
	setAction(static_cast<TextManipulatorAction>(oProfile.value(oKey+"_action").toInt()));

	int val1 = oProfile.value(oKey+"_min_length").toInt();
	int val2 = oProfile.value(oKey+"_max_length").toInt();
	int val3 = oProfile.value(oKey+"_fill_char").toInt();
	bool b = oProfile.value(oKey+"_fill_at_end").toBool();
	setLength(val1, val2, val3, b);

	bool invert = oProfile.value(oKey+"_invert_selection").toBool();
	QList<QPair<int, int>>l;
	toPositionList(oProfile.value(oKey+"_positions").toString(), l);
	setPositions(l);

	b = oProfile.value(oKey+"_regular_expression").toBool();
	setPattern(oProfile.value(oKey+"_regular_expression").toString(), invert, b);

	getConfigurationPanel(NULL);
	setValues();
	return true;
}

void TextManipulator::saveProfile(QSettings &oProfile, QString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);

	readValues();
	oProfile.setValue(oKey+"_text", getText());
	oProfile.setValue(oKey+"_action", getAction());

	oProfile.setValue(oKey+"_min_length", getMinLength());
	oProfile.setValue(oKey+"_max_length", getMinLength());
	oProfile.setValue(oKey+"_fill_char", getFillCharacter());
	oProfile.setValue(oKey+"_fill_at_end", getFillAtEnd());

	QString s;
	toPositionText(getPositions(), s);
	oProfile.setValue(oKey+"_positions", s);

	oProfile.setValue(oKey+"_pattern", getPattern());
	oProfile.setValue(oKey+"_regular_expression", getRegularExpression());
}

void TextManipulator::prepare(void)
{
	Manipulator::prepare();
	readValues();
}

QString TextManipulator::selectByPosition(QString const &oValue) const
{
	QString s;

	QList<QPair<int, int>> l = getPositions();
	int n = oValue.length();
	for(QPair<int, int> &p : l)
	{
		int start = p.first;
		int end = p.second;

		if(start < 1)
			start = 0;
		else
		{
			start --;
			if(end < 1)
			{
				if(end == 0)
				{
					// Single character at position X
					if(start < n)
						s += oValue.at(start);

					continue;
				}
				end = n;
			}
		}

		if(end > n)
			end = n;

		s += oValue.mid(start, end-start);
	}

	return s;
}

QString TextManipulator::selectByPattern(QString const &oValue, QString const &oPattern, bool bRegularExpression, bool bInvertSelection, const QString *oReplace) const
{
	QString s;
	QRegExp regex;

	if(bRegularExpression)
		regex.setPatternSyntax(QRegExp::RegExp);
	else
		regex.setPatternSyntax(QRegExp::WildcardUnix);

	regex.setPattern(oPattern);
	int i = oValue.indexOf(regex);
	QList<QString> matches = regex.capturedTexts();
	int l = regex.matchedLength();
	int sz = matches.size();
	QString match;
	if(sz > 0)
		match = matches.at(0);

	if(bInvertSelection)
	{
		if(i == -1 || l < 1)
			return oValue;

		s = oValue.mid(0, i) + oValue.mid(i+l);
	}
	else
	{
		if(i == -1 || l < 1)
			return s;

		if(oReplace == NULL)
			s = oValue.mid(i, l);
		else
			s = oValue.mid(0, i) + (*oReplace) + oValue.mid(i+l);
	}

	return s;
}

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QString>

#include <support/helper/string.h>
#include <support/unicode/unicode_types.h>

#include "manipulator/text/text.h"
#include "support/helper/string.h"

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
			setError("Internal Error! Invalid Action value:"+ spt::string::toString(getAction()));
			return false;
		}
	}

	return true;
}

StdString TextManipulator::getId(void)
{
	return "DBEF3EA0-FFC0-11E3-9191-0800200C9A66";
}

void TextManipulator::reset(void)
{
	Manipulator::reset();
	setAction(TextManipulatorAction::SEL_Text);
	setText("");
	setLength(0, 0, 0, true);
	setPositions(std::vector<std::pair<int, int>>());
	setPattern("", false, false);
	setValues();
}

void TextManipulator::setText(StdString const &oText)
{
	mText = oText;
}

StdString TextManipulator::getText(void) const
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

bool TextManipulator::setPositions(std::vector<std::pair<int, int>> const &oPositionList)
{
	mPositions = oPositionList;
	return true;
}

std::vector<std::pair<int, int>> TextManipulator::getPositions(void) const
{
	return mPositions;
}

void TextManipulator::setPattern(StdString const &oPattern, bool bInvertSelection, bool bRegularExpression)
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

StdString TextManipulator::getPattern(void) const
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

StdString TextManipulator::getName(void)
{
	return "String formatting";
}

StdString TextManipulator::getDescription(void)
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
	std::vector<std::pair<int, int>>l;
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

StdString *TextManipulator::format(StdString *oValue, bool bPreview)
{
	UNUSED(bPreview);

	StdString s;
	StdString v;

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
					s = s.substr(0, mMaxLength);
			}

			bool end = getFillAtEnd();
			int c = getFillCharacter();
			if(!c)
				c = ' ';

			while(s.length() < (size_t)mMinLength)
			{
				if(end)
					s += c;
				else
					s = static_cast<spt::string::char_t>(c) + s;
			}
		}
		break;

		case TextManipulatorAction::SEL_SelectPattern:
			s = selectByPattern(v, getPattern(), getRegularExpression(), getInvertSelection(), NULL);
		break;

		case TextManipulatorAction::SEL_ReplacePattern:
		{
			StdString replace = getText();
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
				StdString s1 = s;
				std::transform(s1.begin(), s1.end(), s.begin(), ::tolower);
				delete oValue;
				oValue = NULL;
			}
		}
		break;

		case TextManipulatorAction::SEL_ToUpper:
		{
			if(oValue)
			{
				StdString s1 = s;
				std::transform(s1.begin(), s1.end(), s.begin(), ::toupper);
				delete oValue;
				oValue = NULL;
			}
		}
		break;

		case TextManipulatorAction::SEL_Multiline:
		{
			if(!oValue)
				break;

			StdString line = *oValue;
			if(getInvertSelection())
				line = spt::string::unmaskString(line);
			else
				line = spt::string::maskString(line);
			s = line;
		}
		break;

		case TextManipulatorAction::SEL_Text:
		default:
			s = getText();
		break;
	}

	return applyMode(oValue, s);
}

bool TextManipulator::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	if(!Manipulator::loadProfile(oProfile, oKey))
		return false;

	auto key = spt::string::toQt(oKey);

	setText(spt::string::fromQt(oProfile.value(key+"_text", "").toString()));
	setAction(static_cast<TextManipulatorAction>(oProfile.value(key+"_action").toInt()));

	int val1 = oProfile.value(key+"_min_length").toInt();
	int val2 = oProfile.value(key+"_max_length").toInt();
	int val3 = oProfile.value(key+"_fill_char").toInt();
	bool b = oProfile.value(key+"_fill_at_end").toBool();
	setLength(val1, val2, val3, b);

	bool invert = oProfile.value(key+"_invert_selection").toBool();
	std::vector<std::pair<int, int>>l;
	toPositionList(spt::string::fromQt(oProfile.value(key+"_positions").toString()), l);
	setPositions(l);

	b = oProfile.value(key+"_regular_expression").toBool();
	setPattern(spt::string::fromQt(oProfile.value(key+"_regular_expression").toString()), invert, b);

	getConfigurationPanel(NULL);
	setValues();
	return true;
}

void TextManipulator::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);
	auto key = spt::string::toQt(oKey);

	readValues();
	oProfile.setValue(key+"_text", spt::string::toQt(getText()));
	oProfile.setValue(key+"_action", getAction());

	oProfile.setValue(key+"_min_length", getMinLength());
	oProfile.setValue(key+"_max_length", getMinLength());
	oProfile.setValue(key+"_fill_char", getFillCharacter());
	oProfile.setValue(key+"_fill_at_end", getFillAtEnd());

	StdString s;
	toPositionText(getPositions(), s);
	oProfile.setValue(key+"_positions", spt::string::toQt(s));

	oProfile.setValue(key+"_pattern", spt::string::toQt(getPattern()));
	oProfile.setValue(key+"_regular_expression", getRegularExpression());
}

void TextManipulator::prepare(void)
{
	Manipulator::prepare();
	readValues();
}

StdString TextManipulator::selectByPosition(StdString const &oValue) const
{
	StdString s;

	std::vector<std::pair<int, int>> l = getPositions();
	int n = oValue.length();
	for(std::pair<int, int> &p : l)
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

		s += oValue.substr(start, end-start);
	}

	return s;
}

StdString TextManipulator::selectByPattern(StdString const &oValue, StdString const &oPattern, bool bRegularExpression, bool bInvertSelection, const StdString *oReplace) const
{
	StdString s;
	QString value = spt::string::toQt(oValue);
	QRegExp regex;

	if(bRegularExpression)
		regex.setPatternSyntax(QRegExp::RegExp);
	else
		regex.setPatternSyntax(QRegExp::WildcardUnix);

	regex.setPattern(spt::string::toQt(oPattern));
	int i = value.indexOf(regex);
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

		s = spt::string::fromQt(value.mid(0, i) + value.mid(i+l));
	}
	else
	{
		if(i == -1 || l < 1)
			return s;

		if(oReplace == NULL)
			s = oValue.substr(i, l);
		else
			s = oValue.substr(0, i) + (*oReplace) + oValue.substr(i+l);
	}

	return s;
}

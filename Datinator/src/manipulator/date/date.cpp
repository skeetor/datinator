/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "manipulator/date/date.h"
#include "manipulator/date/date_panel_gui.moc"

#include "support/helper/string.h"

static bool initFormats(void);

std::vector<StdString> gFormat;
std::vector<StdString> gFormatText;


namespace datinator_unused {
bool gInitialized = initFormats();
}

// @static
static bool initFormats(void)
{
	if(gFormat.size() != 0)
		return true;

	for(int i = 0; i < DateManipulator::DateFormat::DT_MAX; i++)
	{
		switch(static_cast<DateManipulator::DateFormat>(i))
		{
			case DateManipulator::DateFormat::DT_SHORT:
			{
				gFormat.push_back("yyyy.MM.dd");
				gFormatText.push_back("yyyy.MM.dd");
				break;
			}

			case DateManipulator::DateFormat::DT_LONG:
			{
				gFormat.push_back("yyyy.MM.dd HH:mm:ss");
				gFormatText.push_back("yyyy.MM.dd HH:mm:ss");
				break;
			}

			case DateManipulator::DateFormat::DT_MILI:
			{
				gFormat.push_back("yyyy.MM.dd HH:mm:ss.z");
				gFormatText.push_back("yyyy.MM.dd HH:mm:ss.z");
				break;
			}

			case DateManipulator::DateFormat::DT_CUSTOM:
			{
				gFormat.push_back("");
				gFormatText.push_back("Custom format (see help for details)");
				break;
			}

			case DateManipulator::DateFormat::DT_SYSDATE:
			{
				gFormat.push_back("");
				gFormatText.push_back("System date");
				break;
			}

			case DateManipulator::DateFormat::DT_MAX:
			break;
		}
	}

	return true;
}

std::vector<StdString> const &DateManipulator::getFormats(void)
{
	return gFormat;
}

std::vector<StdString> const &DateManipulator::getFormatTexts(void)
{
	return gFormatText;
}

// @static
StdString DateManipulator::getFormat(DateManipulator::DateFormat nFormat)
{
	StdString s;
	if(nFormat >= DateManipulator::DateFormat::DT_SHORT && nFormat < DateManipulator::DateFormat::DT_MAX)
		s = gFormat[nFormat];

	return s;
}

DateManipulator::DateManipulator(void)
{
	mPanel = NULL;
	reset();
}

DateManipulator::DateManipulator(DateManipulator const &oSource)
{
	mPanel = NULL;
	reset();
	copy(oSource);
}

DateManipulator::~DateManipulator(void)
{
}

bool DateManipulator::isValid(void) const
{
	return true;
}

StdString DateManipulator::getId(void)
{
	return "41714980-046C-11E4-9191-0800200C9A66";
}

void DateManipulator::reset(void)
{
	Manipulator::reset();

	setSysdate(false);
	setSysdateStart(false);
	setInputFormat("yyyy.MM.dd");
	setOutputFormat("yyyy.MM.dd");
	//setInputFormat(DateManipulator::getFormat(DateManipulator::DateFormat::DT_SHORT));
	//setOutputFormat(DateManipulator::getFormat(DateManipulator::DateFormat::DT_SHORT));
	setMode(Manipulator::Mode::REPLACE);
	readValues();
}

DateManipulator &DateManipulator::operator=(DateManipulator const &oSource)
{
	reset();
	copy(oSource);
	return *this;
}

void DateManipulator::copy(DateManipulator const &oSource)
{
	reset();
	Manipulator::copy(oSource);
	setInputFormat(oSource.getInputFormat());
	setOutputFormat(oSource.getOutputFormat());
	setSysdate(oSource.isSysdate());
	setSysdateStart(oSource.isSysdateStart());
	setValues();
}

StdString DateManipulator::getName(void)
{
	return "Date format";
}

StdString DateManipulator::getDescription(void)
{
	return "Returns a date value";
}

IManipulator *DateManipulator::createInstance(void)
{
	return new DateManipulator();
}

IManipulator *DateManipulator::duplicate(void)
{
	return new DateManipulator(*this);
}

void DateManipulator::readValues(void)
{
	if(!mPanel)
		return;

	setMode(mPanel->getMode());
	setSysdate(mPanel->isSysdate());
	setSysdateStart(mPanel->isSysdateStart());
	setInputFormat(mPanel->getInputFormat());
	setOutputFormat(mPanel->getOutputFormat());
}

void DateManipulator::setValues(void)
{
	if(!mPanel)
		return;

	mPanel->setInputFormat(getInputFormat());
	mPanel->setOutputFormat(getOutputFormat());
	mPanel->setSysdate(isSysdate());
	mPanel->setSysdateStart(isSysdateStart());
	mPanel->setMode(getMode());
}

StdString DateManipulator::getInputFormat(void) const
{
	return mInputFormat;
}

void DateManipulator::setInputFormat(StdString const &oFormat)
{
	mInputFormat = oFormat;
}

StdString DateManipulator::getOutputFormat(void) const
{
	return mOutputFormat;
}

void DateManipulator::setOutputFormat(StdString const &oFormat)
{
	mOutputFormat = oFormat;
}

bool DateManipulator::isSysdate(void) const
{
	return mSysdate;
}

void DateManipulator::setSysdate(bool bSysdate)
{
	mSysdate = bSysdate;
}

bool DateManipulator::isSysdateStart(void) const
{
	return mSysdateStart;
}

void DateManipulator::setSysdateStart(bool bSysdateStart)
{
	mSysdateStart = bSysdateStart;
}

QWidget *DateManipulator::getConfigurationPanel(QWidget *oParent)
{
	if(mPanel == NULL)
		mPanel = new DateManipulatorPanel(this, oParent);

	setValues();

	return mPanel;
}

bool DateManipulator::isConfigured(void)
{
	readValues();

	if(getOutputFormat().length() == 0)
		return false;

 	if(!isSysdate() && getInputFormat().length() == 0)
	{
		if(getInputFormat().length() == 0)
			return false;
	}

	return true;
}

StdString DateManipulator::createString(StdString const &oInputDate)
{
	QDateTime t;

 	if(isSysdate())
	{
		if(isSysdateStart())
			t = mDateTime;
		else
			t = QDateTime::currentDateTime();
	}
	else
	{
		t = QDateTime::fromString(spt::string::toQt(oInputDate), spt::string::toQt(getInputFormat()));

		setValid(t.isValid());
		if(!isValid())
			setError("Invalid date format: Input=["+oInputDate+"] Output=["+getOutputFormat()+"]");
	}

	return spt::string::fromQt(t.toString(spt::string::toQt(getOutputFormat())));
}

/**
 * If the input string has some value, it is only used
 * if no Text is specified by the user. If this is the case
 */
StdString *DateManipulator::format(StdString *oValue, bool bPreview)
{
	StdString v;

	if(bPreview)
	{
		QDateTime t = QDateTime::currentDateTime();
		v = spt::string::fromQt(t.toString(spt::string::toQt(getInputFormat())));
	}
	else
	{
		if(oValue)
			v = *oValue;
	}

	v = createString(v);
	return applyMode(oValue, v);
}

void DateManipulator::prepare(void)
{
	Manipulator::prepare();
	readValues();
	mDateTime = QDateTime::currentDateTime();
}

bool DateManipulator::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	if(!Manipulator::loadProfile(oProfile, oKey))
		return false;
	auto key = spt::string::toQt(oKey);

	setSysdate(oProfile.value(key+"_sysdate", "false").toBool());
	setSysdateStart(oProfile.value(key+"_sysdate_start", "false").toBool());
	setInputFormat(spt::string::fromQt(oProfile.value(key+"_input_format", "yyyy.MM.dd").toString()));
	setOutputFormat(spt::string::fromQt(oProfile.value(key+"_output_format", "yyyy.MM.dd").toString()));

	getConfigurationPanel(NULL);
	setValues();
	return true;
}

void DateManipulator::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);
	auto key = spt::string::toQt(oKey);

	readValues();
	oProfile.setValue(key+"_sysdate", isSysdate());
	oProfile.setValue(key+"_sysdate_start", isSysdateStart());
	oProfile.setValue(key+"_input_format", spt::string::toQt(getInputFormat()));
	oProfile.setValue(key+"_output_format", spt::string::toQt(getOutputFormat()));
}

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QString>
#include <QtCore/QSettings>

#include "manipulator/date/date.h"
#include "manipulator/date/date_panel_gui.moc"

bool initFormats(void);

QList<QString> gFormat;
QList<QString> gFormatText;
bool gInitialized = initFormats();

// @static
bool initFormats(void)
{
	if(gFormat.size() != 0)
		return true;

	for(int i = 0; i < DateManipulator::DateFormat::DT_MAX; i++)
	{
		switch(static_cast<DateManipulator::DateFormat>(i))
		{
			case DateManipulator::DateFormat::DT_SHORT:
			{
				gFormat.append("yyyy.MM.dd");
				gFormatText.append("yyyy.MM.dd");
				break;
			}

			case DateManipulator::DateFormat::DT_LONG:
			{
				gFormat.append("yyyy.MM.dd HH:mm:ss");
				gFormatText.append("yyyy.MM.dd HH:mm:ss");
				break;
			}

			case DateManipulator::DateFormat::DT_MILI:
			{
				gFormat.append("yyyy.MM.dd HH:mm:ss.z");
				gFormatText.append("yyyy.MM.dd HH:mm:ss.z");
				break;
			}

			case DateManipulator::DateFormat::DT_CUSTOM:
			{
				gFormat.append("");
				gFormatText.append("Custom format (see help for details)");
				break;
			}

			case DateManipulator::DateFormat::DT_SYSDATE:
			{
				gFormat.append("");
				gFormatText.append("System date");
				break;
			}

			case DateManipulator::DateFormat::DT_MAX:
			break;
		}
	}

	return true;
}

QList<QString> const &DateManipulator::getFormats(void)
{
	return gFormat;
}

QList<QString> const &DateManipulator::getFormatTexts(void)
{
	return gFormatText;
}

// @static
QString DateManipulator::getFormat(DateManipulator::DateFormat nFormat)
{
	QString s;
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

QString DateManipulator::getId(void)
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

QString DateManipulator::getName(void)
{
	return "Date format";
}

QString DateManipulator::getDescription(void)
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

QString DateManipulator::getInputFormat(void) const
{
	return mInputFormat;
}

void DateManipulator::setInputFormat(QString const &oFormat)
{
	mInputFormat = oFormat;
}

QString DateManipulator::getOutputFormat(void) const
{
	return mOutputFormat;
}

void DateManipulator::setOutputFormat(QString const &oFormat)
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

QString DateManipulator::createString(QString const &oInputDate)
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
		t = QDateTime::fromString(oInputDate, getInputFormat());

		setValid(t.isValid());
		if(!isValid())
			setError("Invalid date format: Input=["+oInputDate+"] Output=["+getOutputFormat()+"]");
	}

	return t.toString(getOutputFormat());
}

/**
 * If the input string has some value, it is only used
 * if no Text is specified by the user. If this is the case
 */
QString *DateManipulator::format(QString *oValue, bool bPreview)
{
	QString v;

	if(bPreview)
	{
		QDateTime t = QDateTime::currentDateTime();
		v = t.toString(getInputFormat());
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

bool DateManipulator::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!Manipulator::loadProfile(oProfile, oKey))
		return false;

	setSysdate(oProfile.value(oKey+"_sysdate", "false").toBool());
	setSysdateStart(oProfile.value(oKey+"_sysdate_start", "false").toBool());
	setInputFormat(oProfile.value(oKey+"_input_format", "yyyy.MM.dd").toString());
	setOutputFormat(oProfile.value(oKey+"_output_format", "yyyy.MM.dd").toString());

	getConfigurationPanel(NULL);
	setValues();
	return true;
}

void DateManipulator::saveProfile(QSettings &oProfile, QString const &oKey)
{
	Manipulator::saveProfile(oProfile, oKey);

	readValues();
	oProfile.setValue(oKey+"_sysdate", isSysdate());
	oProfile.setValue(oKey+"_sysdate_start", isSysdateStart());
	oProfile.setValue(oKey+"_input_format", getInputFormat());
	oProfile.setValue(oKey+"_output_format", getOutputFormat());
}

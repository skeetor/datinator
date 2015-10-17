/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef DATE_MANIPULATOR_H_INCLUDED
#define DATE_MANIPULATOR_H_INCLUDED

#include <QtCore/QDateTime>

#include "manipulator/manipulator.h"

class DateManipulatorPanel;

class DateManipulator
	: public Manipulator
{
public:
	typedef enum
	{
		DT_SHORT,
		DT_LONG,
		DT_MILI,
		DT_CUSTOM,
		DT_SYSDATE,
		DT_MAX
	} DateFormat;


public:
	DateManipulator(void);
	DateManipulator(DateManipulator const &oSource);
	virtual ~DateManipulator(void);

	StdString getId(void) override;
	StdString getName(void) override;
	StdString getDescription(void) override;
	IManipulator *createInstance(void) override;
	IManipulator *duplicate(void) override;
	QWidget *getConfigurationPanel(QWidget *oParent = 0) override;
	StdString *format(StdString *oValue, bool bPreview = false) override;
	void reset(void) override;
	void prepare(void) override;
	bool isConfigured(void) override;
	bool isValid(void) const override;

	/**
	 * Return the format of the specified type. This only returns valid
	 * formats if it is not DT_SYSDATE or DT_CUSTOM.
	 */
	static StdString getFormat(DateManipulator::DateFormat nFormat);
	static std::vector<StdString> const &getFormats(void);
	static std::vector<StdString> const &getFormatTexts(void);

	virtual DateManipulator &operator=(DateManipulator const &oSource);
	virtual void copy(DateManipulator const &oSource);

	StdString getInputFormat(void) const;
	void setInputFormat(StdString const &oFormat);

	StdString getOutputFormat(void) const;
	void setOutputFormat(StdString const &oFormat);

	bool isSysdate(void) const;
	void setSysdate(bool bSysdate = true);

	bool isSysdateStart(void) const;
	void setSysdateStart(bool bSysdateStart = true);

	void saveProfile(QSettings &oProfile, StdString const &oKey) override;
	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;

protected:
	/**
	 * Read the values from the GUI panel into the variables.
	 */
	void readValues(void) override;
	void setValues(void) override;
	StdString createString(StdString const &oInputDate);

private:
	DateManipulatorPanel *mPanel;
	bool mSysdate;
	bool mSysdateStart;
	StdString mInputFormat;
	StdString mOutputFormat;
	QDateTime mDateTime;
};

#endif // DATE_MANIPULATOR_H_INCLUDED

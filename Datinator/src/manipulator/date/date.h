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

	QString getId(void) override;
	QString getName(void) override;
	QString getDescription(void) override;
	IManipulator *createInstance(void) override;
	IManipulator *duplicate(void) override;
	QWidget *getConfigurationPanel(QWidget *oParent = 0) override;
	QString *format(QString *oValue, bool bPreview = false) override;
	void reset(void) override;
	void prepare(void) override;
	bool isConfigured(void) override;
	bool isValid(void) const override;

	/**
	 * Return the format of the specified type. This only returns valid
	 * formats if it is not DT_SYSDATE or DT_CUSTOM.
	 */
	static QString getFormat(DateManipulator::DateFormat nFormat);
	static QList<QString> const &getFormats(void);
	static QList<QString> const &getFormatTexts(void);

	virtual DateManipulator &operator=(DateManipulator const &oSource);
	virtual void copy(DateManipulator const &oSource);

	QString getInputFormat(void) const;
	void setInputFormat(QString const &oFormat);

	QString getOutputFormat(void) const;
	void setOutputFormat(QString const &oFormat);

	bool isSysdate(void) const;
	void setSysdate(bool bSysdate = true);

	bool isSysdateStart(void) const;
	void setSysdateStart(bool bSysdateStart = true);

	void saveProfile(QSettings &oProfile, QString const &oKey) override;
	bool loadProfile(QSettings &oProfile, QString const &oKey) override;

protected:
	/**
	 * Read the values from the GUI panel into the variables.
	 */
	void readValues(void) override;
	void setValues(void) override;
	QString createString(QString const &oInputDate);

private:
	DateManipulatorPanel *mPanel;
	bool mSysdate;
	bool mSysdateStart;
	QString mInputFormat;
	QString mOutputFormat;
	QDateTime mDateTime;
};

#endif // DATE_MANIPULATOR_H_INCLUDED

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef MANIPULATOR_H_INCLUDED
#define MANIPULATOR_H_INCLUDED

#include "manipulator/imanipulator.h"

class Manipulator
	:	public virtual IManipulator
	,	Dispatcher<IManipulator *>
{
public:

	typedef enum
	{
		PREPEND,
		APPEND,
		REPLACE
	} Mode;

public:
	Manipulator(void);
	virtual ~Manipulator(void);

	virtual void copy(Manipulator const &oSource);

	QString *applyMode(QString *oValue, QString const &oManipulatorString);

	void addConfigChangeListener(Listener<IManipulator *> *oListener) override;
	void removeConfigChangeListener(Listener<IManipulator *> *oListener) override;
	void prepare(void) override;
	void reset(void) override;
	bool isValid(void) const override;
	QString lastError(void) override;

	void setSourceColumns(QList<DatabaseColumn *> const &oColumns) override;
	QList<DatabaseColumn *> getSourceColumns(void) const;

	void saveProfile(QSettings &oProfile, QString const &oKey) override;
	bool loadProfile(QSettings &oProfile, QString const &oKey) override;

	void notifyConfigChangedListeners(IManipulator *oManipulator);

	Manipulator::Mode getMode(void) const;
	void setMode(Manipulator::Mode nMode);

	QString getTestValue(void) const override { return mTestValue; };
	void setTestValue(QString const &oValue) override { mTestValue = oValue; };

	virtual QString toString(void);

protected:
	void setValid(bool bValid = true);
	void setError(QString const &oText);

	virtual void readValues(void);
	virtual void setValues(void);

private:
	Manipulator::Mode mMode;
	bool mValid;
	QList<DatabaseColumn *> mColumns;
	QString mErrorText;
	QString mTestValue;
};

#endif // MANIPULATOR_H_INCLUDED

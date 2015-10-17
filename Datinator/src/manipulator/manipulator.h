/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef MANIPULATOR_H_INCLUDED
#define MANIPULATOR_H_INCLUDED

#include "manipulator/imanipulator.h"
#include "datinator_types.h"

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

	StdString *applyMode(StdString *oValue, StdString const &oManipulatorString);

	void addConfigChangeListener(Listener<IManipulator *> *oListener) override;
	void removeConfigChangeListener(Listener<IManipulator *> *oListener) override;
	void prepare(void) override;
	void reset(void) override;
	bool isValid(void) const override;
	StdString lastError(void) override;

	void setSourceColumns(std::vector<DatabaseColumn *> const &oColumns) override;
	std::vector<DatabaseColumn *> getSourceColumns(void) const;

	void saveProfile(QSettings &oProfile, StdString const &oKey) override;
	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;

	void notifyConfigChangedListeners(IManipulator *oManipulator);

	Manipulator::Mode getMode(void) const;
	void setMode(Manipulator::Mode nMode);

	StdString getTestValue(void) const override { return mTestValue; };
	void setTestValue(StdString const &oValue) override { mTestValue = oValue; };

	virtual StdString toString(void);

protected:
	void setValid(bool bValid = true);
	void setError(StdString const &oText);

	virtual void readValues(void);
	virtual void setValues(void);

private:
	Manipulator::Mode mMode;
	bool mValid;
	std::vector<DatabaseColumn *> mColumns;
	StdString mErrorText;
	StdString mTestValue;
};

#endif // MANIPULATOR_H_INCLUDED

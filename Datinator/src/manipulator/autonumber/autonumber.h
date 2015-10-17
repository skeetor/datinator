/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef _AUTONUMBER_MANIPULATOR_H_INCLUDED
#define _AUTONUMBER_MANIPULATOR_H_INCLUDED

#include "manipulator/manipulator.h"
#include "manipulator/autonumber/autonumber_panel_gui.moc"

class AutoNumberManipulator
	: public Manipulator
{
public:
	AutoNumberManipulator(void);
	AutoNumberManipulator(AutoNumberManipulator const &oSource);
	virtual ~AutoNumberManipulator(void);

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

	virtual AutoNumberManipulator &operator=(AutoNumberManipulator const &oSource);
	virtual void copy(AutoNumberManipulator const &oSource);

	size_t getValue(void) const;
	void setValue(size_t nValue);

	size_t getWidth(void) const;
	void setWidth(size_t nWidth);

	size_t getIncrement(void) const;
	void setIncrement(size_t bIncrement);

	bool getLeadingZeroes(void) const;
	void setLeadingZeroes(bool bLeadingZeroes);

	void saveProfile(QSettings &oProfile, StdString const &oKey) override;
	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;

protected:
	/**
	 * Read the values from the GUI panel size_to the variables.
	 */
	void readValues(void);
	void setValues(void);
	StdString createString(void) const;

private:
	void init(void);

private:
	AutoNumberPanel *mPanel;
	size_t mValue;
	size_t mWidth;
	size_t mIncrement;
	bool mLeadingZeroes;
};

#endif // _AUTONUMBER_MANIPULATOR_H_INCLUDED

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef AUTONUMBER_MANIPULATOR_H_INCLUDED
#define AUTONUMBER_MANIPULATOR_H_INCLUDED

#include "manipulator/manipulator.h"
#include "manipulator/autonumber/autonumber_panel_gui.moc"

class AutoNumberManipulator
	: public Manipulator
{
public:
	AutoNumberManipulator(void);
	AutoNumberManipulator(AutoNumberManipulator const &oSource);
	virtual ~AutoNumberManipulator(void);

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

	virtual AutoNumberManipulator &operator=(AutoNumberManipulator const &oSource);
	virtual void copy(AutoNumberManipulator const &oSource);

	int getValue(void) const;
	void setValue(int nValue);

	int getWidth(void) const;
	void setWidth(int nWidth);

	int getIncrement(void) const;
	void setIncrement(int bIncrement);

	bool getLeadingZeroes(void) const;
	void setLeadingZeroes(bool bLeadingZeroes);

	void saveProfile(QSettings &oProfile, QString const &oKey) override;
	bool loadProfile(QSettings &oProfile, QString const &oKey) override;

protected:
	/**
	 * Read the values from the GUI panel into the variables.
	 */
	void readValues(void);
	void setValues(void);
	QString createString(void) const;

private:
	void init(void);

private:
	AutoNumberPanel *mPanel;
	int mValue;
	int mWidth;
	int mIncrement;
	bool mLeadingZeroes;
};

#endif // AUTONUMBER_MANIPULATOR_H_INCLUDED

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef COLUMN_MANIPULATOR_H_INCLUDED
#define COLUMN_MANIPULATOR_H_INCLUDED

#include "manipulator/manipulator.h"
#include "manipulator/column/column_panel_gui.moc"

class ColumnManipulatorPanel;

class ColumnManipulator
	: public Manipulator
{
public:
	ColumnManipulator(void);
	ColumnManipulator(ColumnManipulator const &oSource);
	virtual ~ColumnManipulator(void);

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

	void saveProfile(QSettings &oProfile, QString const &oKey) override;
	bool loadProfile(QSettings &oProfile, QString const &oKey) override;

	virtual ColumnManipulator &operator=(ColumnManipulator const &oSource);
	virtual void copy(ColumnManipulator const &oSource);

	void setColumn(int nColumnIndex);
	int getColumn(void) const;

protected:
	/**
	 * Read the values from the GUI panel into the variables.
	 */
	void readValues(void) override;
	void setValues(void) override;

private:
	ColumnManipulatorPanel *mPanel;
	int mColumnIndex;
};

#endif // COLUMN_MANIPULATOR_H_INCLUDED

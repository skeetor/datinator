/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef TEXT_MANIPULATOR_H_INCLUDED
#define TEXT_MANIPULATOR_H_INCLUDED

#include "manipulator/manipulator.h"
#include "manipulator/text/text_panel_gui.moc"
#include "manipulator/text/selection.h"

class TextManipulatorPanel;

class TextManipulator
	: public Manipulator
{
public:
	TextManipulator(void);
	TextManipulator(TextManipulator const &oSource);
	virtual ~TextManipulator(void);

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

	virtual TextManipulator &operator=(TextManipulator const &oSource);
	virtual void copy(TextManipulator const &oSource);

	void setAction(TextManipulatorAction nAction);
	TextManipulatorAction getAction(void) const;

	void setLength(int nMinLength, int nMaxLength, int nFillCharacter, bool bFillAtEnd);
	int getMinLength(void) const;
	int getMaxLength(void) const;
	int getFillCharacter(void) const;
	bool getFillAtEnd(void) const;

	bool setPositions(QList<QPair<int, int>> const &oPositionList);
	QList<QPair<int, int>> getPositions(void) const;

	void setPattern(QString const &oPattern, bool bInvertSelection, bool bRegularExpression);
	bool getInvertSelection(void) const;
	bool getRegularExpression(void) const;
	QString getPattern(void) const;

	void setText(QString const &oText);
	QString getText(void) const;

protected:
	/**
	 * Read the values from the GUI panel into the variables.
	 */
	void readValues(void) override;
	void setValues(void) override;

	/**
	 * Select the string indicated by the position settings
	 */
	QString selectByPosition(QString const &oValue) const;

	/**
	 * Select the string indicated by a regular expression/wildcard pattern.
	 */
	QString selectByPattern(QString const &oValue, QString const &oPattern, bool bRegularExpression, bool bInvertSelection, const QString *oReplace) const;

private:
	TextManipulatorPanel *mPanel;
	TextManipulatorAction mAction;
	int mMinLength;
	int mMaxLength;
	int mFillCharacter;
	bool mFillAtEnd;
	bool mInvertSelection;
	bool mRegularExpression;
	QList<QPair<int, int>> mPositions;
	QString mPattern;
	QString mText;
};

#endif // TEXT_MANIPULATOR_H_INCLUDED

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef TEXT_MANIPULATOR_H_INCLUDED
#define TEXT_MANIPULATOR_H_INCLUDED

#include <utility>

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

	void saveProfile(QSettings &oProfile, StdString const &oKey) override;
	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;

	virtual TextManipulator &operator=(TextManipulator const &oSource);
	virtual void copy(TextManipulator const &oSource);

	void setAction(TextManipulatorAction nAction);
	TextManipulatorAction getAction(void) const;

	void setLength(int nMinLength, int nMaxLength, int nFillCharacter, bool bFillAtEnd);
	int getMinLength(void) const;
	int getMaxLength(void) const;
	int getFillCharacter(void) const;
	bool getFillAtEnd(void) const;

	bool setPositions(std::vector<std::pair<int, int>> const &oPositionList);
	std::vector<std::pair<int, int>> getPositions(void) const;

	void setPattern(StdString const &oPattern, bool bInvertSelection, bool bRegularExpression);
	bool getInvertSelection(void) const;
	bool getRegularExpression(void) const;
	StdString getPattern(void) const;

	void setText(StdString const &oText);
	StdString getText(void) const;

protected:
	/**
	 * Read the values from the GUI panel into the variables.
	 */
	void readValues(void) override;
	void setValues(void) override;

	/**
	 * Select the string indicated by the position settings
	 */
	StdString selectByPosition(StdString const &oValue) const;

	/**
	 * Select the string indicated by a regular expression/wildcard pattern.
	 */
	StdString selectByPattern(StdString const &oValue, StdString const &oPattern, bool bRegularExpression, bool bInvertSelection, const StdString *oReplace) const;

private:
	TextManipulatorPanel *mPanel;
	TextManipulatorAction mAction;
	ssize_t mMinLength;
	ssize_t mMaxLength;
	int mFillCharacter;
	bool mFillAtEnd;
	bool mInvertSelection;
	bool mRegularExpression;
	std::vector<std::pair<int, int>> mPositions;
	StdString mPattern;
	StdString mText;
};

#endif // TEXT_MANIPULATOR_H_INCLUDED

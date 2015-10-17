/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

/**
 * This formatter will always set the input value to null.
 *
 * @author Gerhard.Gruber
 *
 */
#ifndef NULL_H_INCLUDED
#define NULL_H_INCLUDED

#include "manipulator/manipulator.h"

class NullManipulator
	: public Manipulator
{
public:
	NullManipulator(void);
	virtual ~NullManipulator(void);

	bool isConfigured(void) override;
	StdString getId(void) override;
	StdString toString(void) override;
	StdString getName(void) override;
	StdString getDescription(void) override;
	IManipulator *createInstance(void) override;
	IManipulator *duplicate(void) override;
	QWidget *getConfigurationPanel(QWidget *oParent = 0) override;
	StdString *format(StdString *oValue, bool bPreview = false) override;
	void reset(void) override;
	void prepare(void) override;

	void saveProfile(QSettings &oProfile, StdString const &oKey) override;
	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
};

#endif // NULL_H_INCLUDED

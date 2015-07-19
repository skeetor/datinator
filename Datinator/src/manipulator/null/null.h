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
	QString getId(void) override;
	QString toString(void) override;
	QString getName(void) override;
	QString getDescription(void) override;
	IManipulator *createInstance(void) override;
	IManipulator *duplicate(void) override;
	QWidget *getConfigurationPanel(QWidget *oParent = 0) override;
	QString *format(QString *oValue, bool bPreview = false) override;
	void reset(void) override;
	void prepare(void) override;

	void saveProfile(QSettings &oProfile, QString const &oKey) override;
	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
};

#endif // NULL_H_INCLUDED

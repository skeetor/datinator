/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

/*******************************************************************************
 * IManipulator is the interface for modifiying data when it is copied.
 *
 ******************************************************************************/

#ifndef IMANIPULATOR_H_INCLUDED
#define IMANIPULATOR_H_INCLUDED

#include <QtCore/QList>

#include "datinator_types.h"
#include "support/patterns/observer.h"

class QString;
class QWidget;
class QSettings;

class IManipulator
{
public:
	IManipulator(void) {};
	virtual ~IManipulator(void) {};

	/**
	 * Return a unique Id identifying the manipulator type (not the instance).
	 */
	virtual QString getId(void) = 0;

	/**
	 * Create a string representation of the manipulator
	 */
	virtual QString toString(void) = 0;

	/**
	 * Returns true if the manipulator is properly configured.
	 * lastError() should return a string explaining the problem.
	 */
	virtual bool isConfigured(void) = 0;

	/**
	 * The string which is displayed to the user in a selection.
	 */
	virtual QString getName(void) = 0;

	/**
	 * This string can be displayed as a helptext to explain the
	 * function of the manipulator.
	 */
	virtual QString getDescription(void) = 0;

	/**
	 * Create an instance of the current manipulator
	 */
	virtual IManipulator *createInstance(void) = 0;

	/**
	 * Create a copy of the current manipulator
	 */
	virtual IManipulator *duplicate(void) = 0;

	/**
	 * If no configuration is necessary or displayed
	 * in a GUI, NULL can be returned.
	 */
	virtual QWidget *getConfigurationPanel(QWidget *oParent = 0) = 0;

	/**
	 * Convert the input value into an output value.
	 * The caller is responsible for deleting the string
	 * after it is no longer needed. This function my return
	 * NULL to indicate a NULL value, as opposed to an empty string.
	 */
	virtual QString *format(QString *oValue, bool bPreview = false) = 0;

	/**
	 * Reset the configuration to it's default values which were specified
	 * by the user. This is not the same as initializing the manipulator.
	 */
	virtual void reset(void) = 0;

	/**
	 * This is called before the formatting function is can be used, to allow
	 * the manipulator to do some initialization first.
	 */
	virtual void prepare(void) = 0;

	/**
	 * The listener is notified, when the configuration has been changed.
	 */
	virtual void addConfigChangeListener(Listener<IManipulator *> *oListener) = 0;
	virtual void removeConfigChangeListener(Listener<IManipulator *> *oListener) = 0;

	/**
	 * This should be called after format() has been used, to check
	 * if the result is a valid result. If false is returned a description
	 * can be obtained by lastError();
	 */
	virtual bool isValid(void) const = 0;

	/**
	 * If isValid() returned false, this should return a text explaining
	 * the problem.
	 */
	virtual QString lastError(void) = 0;

	/**
	 * The test value is a user defined value whcih can be set to allow preview
	 * and testing of the effects of the manipulator
	 */
	virtual QString getTestValue(void) const = 0;
	virtual void setTestValue(QString const &oValue) = 0;

	virtual bool loadProfile(QSettings &oProfile, QString const &oKey) = 0;
	virtual void saveProfile(QSettings &oProfile, QString const &oKey) = 0;

	virtual void setSourceColumns(QList<DatabaseColumn *> const &oColumns) = 0;
};

#endif // IMANIPULATOR_H_INCLUDED

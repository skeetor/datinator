/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef COLUMN_MAPPING_COMBOBOX_H
#define COLUMN_MAPPING_COMBOBOX_H

#include <QtWidgets/QComboBox>

#include <support/patterns/observer.h>
#include <support/db/dbcolumn.h>
#include "plugins/datinator_types.h"

class ColumnMappingCombobox :
	 public QComboBox,
	 public Listener<QList<DatabaseColumn *> *>
{
public:
	ColumnMappingCombobox(QWidget *oParent = NULL);
	virtual ~ColumnMappingCombobox(void);

	void setDispatcher(Dispatcher<QList<DatabaseColumn *> *> *oDispatcher);
	void setColumns(QList<DatabaseColumn *> const &oColumns);

public:	// Listener<T>
	virtual void handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oColumns);
	virtual void invalidateDispatcher(Dispatcher<QList<DatabaseColumn *> *> const *oDispatcher);

protected:
	void updateColumns(QList<DatabaseColumn *> const &oColumns);

private:
	Dispatcher<QList<DatabaseColumn *> *> *mDispatcher;
};

#endif // COLUMN_MAPPING_COMBOBOX_H

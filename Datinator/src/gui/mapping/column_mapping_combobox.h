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
	 public Listener<std::vector<DatabaseColumn *> *>
{
public:
	ColumnMappingCombobox(QWidget *oParent = NULL);
	virtual ~ColumnMappingCombobox(void);

	void setDispatcher(Dispatcher<std::vector<DatabaseColumn *> *> *oDispatcher);
	void setColumns(std::vector<DatabaseColumn *> const &oColumns);

public:	// Listener<T>
	virtual void handleNotification(Dispatcher<std::vector<DatabaseColumn *> *> *oSource, std::vector<DatabaseColumn *> *oColumns);
	virtual void invalidateDispatcher(Dispatcher<std::vector<DatabaseColumn *> *> const *oDispatcher);

protected:
	void updateColumns(std::vector<DatabaseColumn *> const &oColumns);

private:
	Dispatcher<std::vector<DatabaseColumn *> *> *mDispatcher;
};

#endif // COLUMN_MAPPING_COMBOBOX_H

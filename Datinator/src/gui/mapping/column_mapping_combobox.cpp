/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QVariant>

#include "gui/mapping/column_mapping_combobox.h"
#include "support/unicode/unicode_types.h"
#include "support/db/dbcolumn.h"

Q_DECLARE_METATYPE(DatabaseColumn *)

ColumnMappingCombobox::ColumnMappingCombobox(QWidget *oParent)
	: QComboBox(oParent)
{
	mDispatcher = NULL;
}

ColumnMappingCombobox::~ColumnMappingCombobox(void)
{
	if(mDispatcher)
		mDispatcher->removeListener(this);
}

void ColumnMappingCombobox::invalidateDispatcher(Dispatcher<QList<DatabaseColumn *> *> const *oDispatcher)
{
	UNUSED(oDispatcher);
	mDispatcher = NULL;
}

void ColumnMappingCombobox::setDispatcher(Dispatcher<QList<DatabaseColumn *> *> *oDispatcher)
{
	mDispatcher = oDispatcher;
	mDispatcher->addListener(this);
}

void ColumnMappingCombobox::handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oColumns)
{
	UNUSED(oSource);

	QList<DatabaseColumn *> columns;
	if(oColumns == NULL)
		oColumns = &columns;

	updateColumns(*oColumns);
}

void ColumnMappingCombobox::setColumns(QList<DatabaseColumn *> const &oColumns)
{
	updateColumns(oColumns);
}

void ColumnMappingCombobox::updateColumns(QList<DatabaseColumn *> const &oColumns)
{
	clear();
	QVariant v;
	v.setValue(NULL);
	addItem("", v);

	for(DatabaseColumn * const &col : oColumns)
	{
		v.setValue(col);
		addItem(col->getName(), v);
	}
	setCurrentIndex(0);
}

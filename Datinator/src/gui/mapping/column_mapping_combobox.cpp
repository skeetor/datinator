/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QVariant>
#include <QtCore/QString>

#include "gui/mapping/column_mapping_combobox.h"
#include "support/unicode/unicode_types.h"
#include "support/db/dbcolumn.h"
#include "support/helper/string.h"

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

void ColumnMappingCombobox::invalidateDispatcher(Dispatcher<std::vector<DatabaseColumn *> *> const *oDispatcher)
{
	UNUSED(oDispatcher);
	mDispatcher = NULL;
}

void ColumnMappingCombobox::setDispatcher(Dispatcher<std::vector<DatabaseColumn *> *> *oDispatcher)
{
	mDispatcher = oDispatcher;
	mDispatcher->addListener(this);
}

void ColumnMappingCombobox::handleNotification(Dispatcher<std::vector<DatabaseColumn *> *> *oSource, std::vector<DatabaseColumn *> *oColumns)
{
	UNUSED(oSource);

	std::vector<DatabaseColumn *> columns;
	if(oColumns == NULL)
		oColumns = &columns;

	updateColumns(*oColumns);
}

void ColumnMappingCombobox::setColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	updateColumns(oColumns);
}

void ColumnMappingCombobox::updateColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	clear();
	QVariant v;
	v.setValue(NULL);
	addItem("", v);

	for(DatabaseColumn * const &col : oColumns)
	{
		v.setValue(col);
		addItem(spt::string::toQt(col->getName()), v);
	}
	setCurrentIndex(0);
}

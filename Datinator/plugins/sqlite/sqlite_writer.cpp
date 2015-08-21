/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "sqlite/sqlite_writer.h"
#include "support/unicode/unicode_types.h"
#include "plugin/gui/progress.h"
#include "sqlite/sqlite_global.h"

#define MODULENAME "SQlite3"

SQLiteWriter::SQLiteWriter(QWidget *oMainWindow)
: SQLiteContainer(oMainWindow)
{
	mConfigPanel = NULL;
	setContainerUUID(SQLITE_WRITER_ID);
}

SQLiteWriter::~SQLiteWriter(void)
{
}

bool SQLiteWriter::canModifyColumns(void) const
{
	return true;
}

bool SQLiteWriter::canTruncate(void) const
{
	return true;
}

bool SQLiteWriter::isReader(void)
{
	return false;
}

SQLiteWriterPanel *SQLiteWriter::createConfigPanel(void)
{
	if(mConfigPanel == NULL)
		mConfigPanel = new SQLiteWriterPanel();

	return mConfigPanel;
}

void SQLiteWriter::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	SQLiteContainer::store(oPropertyFile, oPrefix);
}

void SQLiteWriter::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	SQLiteContainer::restore(oPropertyFile, oPrefix);
}

void SQLiteWriter::commit(void)
{
	SociContainer::commit();
}

void SQLiteWriter::rollback(void)
{
	SociContainer::rollback();
}

bool SQLiteWriter::loadProfile(QSettings &oProfile, QString const &oKey)
{
	createConfigPanel();
	if(!SQLiteContainer::loadProfile(oProfile, oKey))
		return false;

	if(!mConfigPanel->loadProfile(oProfile, oKey))
		return false;

	return true;
}

void SQLiteWriter::saveProfile(QSettings &oProfile, QString const &oKey)
{
	SQLiteContainer::saveProfile(oProfile, oKey);
	mConfigPanel->saveProfile(oProfile, oKey);
}

bool SQLiteWriter::updateTable(QList<DatabaseColumn *> const &oColumns)
{
	// In the writer we are always writing to a table.
//	if(!isQueryByTable())
//		return true;

	QList<DatabaseColumn *> required;
	bool table_missing = false;
	QString container_table = getTablename().toUpper();

	if(container_table.length() == 0)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Tablename is empty! Please enter a tablename or select an existing table.");
		return false;
	}

	StdString table_name = supportlib::string::QtStringToStringT(container_table);

	QList<DatabaseColumn *> tabColumns = readColumnsFromTable(container_table);
	if(tabColumns.size() == 0)
	{
		required = oColumns;
		table_missing = true;
	}
	else
	{
		// Check if all columns of the target are available in the
		// table. If not we may have to add the.
		for(DatabaseColumn * const &col : oColumns)
		{
			QString cn = col->getName().toUpper();
			bool found = false;
			for(DatabaseColumn * &tab_col : tabColumns)
			{
				QString tn = tab_col->getName().toUpper();
				if(cn == tn)
				{
					found = true;
					break;
				}
			}

			if(!found)
				required.append(col);
		}
	}

	if(required.size() > 0)
	{
		for(DatabaseColumn * const &col : oColumns)
		{
			QString cn = col->getName();
			for(int i = 0; i < cn.length(); i++)
			{
				int c = cn.at(i).toLatin1();
				bool invalid = false;
				if(!isalpha(c))
				{
					if(isdigit(c))
					{
						if(i == 0)
							invalid = true;
					}
					else if(c != '_' && c != '$')
						invalid = true;
				}
				if(invalid)
				{
					ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "[" + cn + "] is an invalid columnname. Please use only [A-Z a-z 0-9 _$]. First character must not be a number.");
					return false;
				}
			}
		}

		SQLiteWriterPanel *p = createConfigPanel();
		bool create = p->getCreateTable();
		bool modify = p->getCreateColumns();
		bool recreate = p->getRecreateTable();

		// If we have missing columns but we are not allowed to create
		// them, then we can't proceed.
        if(!create && !modify)
		{
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Can't create target columns or target table '"+container_table+"'");
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Please active the 'Create Table', and/or 'Create Columns' option, choose a matching table or choose appropriate columns.");
			return false;
		}

		if(recreate)
			table_missing = true;
		else
		{
			// If the table is missing and we are not allowed to create it,
			// then we can't proceed.
			if(table_missing && !create)
			{
				ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Can't create target columns or target table '"+container_table+"'");
				ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Please active the 'Create Table', and/or 'Create Columns' option, choose a matching table or choose appropriate columns.");
				return false;
			}
		}

		StdString statement;
		if(!getSession())
			return false;

		soci::session &session = *getSession();
		try
		{
			StdString typ;
			if(table_missing)
			{
				// Either the table doesn't exist, or it should be recreated.
				try
				{
					statement = "drop table "+table_name;
					session << statement;
				}
				catch(std::runtime_error const &e)
				{
				}

				statement = "create table "+table_name + " (\n";
				StdString sep = ",\n";
				for(DatabaseColumn * const &col : required)
				{
					switch(col->getType())
					{
						case supportlib::db::DataType::type_integer:
						case supportlib::db::DataType::type_decimal:
							typ = "INTEGER";
						break;

						default:
							typ	= "TEXT";
					}

					statement += supportlib::string::QtStringToStringT(col->getName()) + " " + typ+sep;
				}
				statement = statement.substr(0, statement.length()-sep.length());
				statement += ")\n";
				session << statement;
			}
			else
			{
				for(DatabaseColumn * const &col : required)
				{
					switch(col->getType())
					{
						case supportlib::db::DataType::type_integer:
						case supportlib::db::DataType::type_decimal:
							typ = "INTEGER";
						break;

						default:
							typ	= "TEXT";
					}

					statement = "alter table " + table_name + " add column " + supportlib::string::QtStringToStringT(col->getName()) + " " + typ;
					session << statement;
				}
			}
			refreshTables(container_table, false);
		}
		catch(std::runtime_error const &e)
		{
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to update tables definition for "+getTablename());
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, supportlib::string::StringTToQtString(statement));
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
			return false;
		}
	}

	return true;
}

bool SQLiteWriter::prepareOpen(QList<DatabaseColumn *> const &oColumns)
{
	setSQLLog(createConfigPanel()->getPath(), createConfigPanel()->getExportSQL(), createConfigPanel()->getExportSQLOnly());
	if(!SociContainer::prepareOpen(oColumns))
		return false;

	if(!updateTable(oColumns))
		return false;

	if(!getSession())
		return false;

	SQLiteWriterPanel *wp = createConfigPanel();
	setAutocommit(wp->getAutoCommitCount());

	soci::session &session = *getSession();
	try
	{
		if(truncateMode())
		{
			// SQLite has an optimization that you can delete without
			// a where clause which performs a truncate.
			// This will not reset autoincrement counters though,
			// so if this is desired, the table should be dropped
			// and recreated, which would probably kill triggers
			// and indexes as well.
			StdString query = "delete from " + supportlib::string::QtStringToStringT(getTablename().toUpper());
			try
			{
				session << query;
			}
			catch(std::runtime_error const &e)
			{
				ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Truncate failed for "+getTablename());
				ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
				return false;
			}
		}
		session << "BEGIN TRANSACTION";
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to start transaction");
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
		return false;
	}

	return true;
}

bool SQLiteWriter::toInsertValue(DatabaseColumn const *oColumn, QString const &oValue, QString &oOutValue)
{
	oOutValue = "";
	if(oColumn->isNull())
	{
		oOutValue = "null";
		return true;
	}

	oOutValue = "'"+oValue+"'";
	return true;
}

int SQLiteWriter::write(QList<DatabaseColumn *> const &oColumns, QList<QString> const &oRow)
{
	return SociContainer::write(oColumns, oRow);
}

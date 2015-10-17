/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QDateTime>

#include "support/helper/string.h"

#include "support_qt/logging/logging_item_model.h"
#include "support/logging/logging.h"

Q_DECLARE_METATYPE(spt::logging::LoggingItem)

typedef enum
{
	LOG_TIMESTAMP,
	LOG_SEVERITY,
	LOG_UNIT,
	LOG_MESSAGE,
	LOG_OBJECT,
	LOG_MAX
} ItemColumns;

LoggingItemModel::LoggingItemModel(QObject *oParent)
: QStandardItemModel(oParent)
{
	setHorizontalHeaderItem(LOG_TIMESTAMP, new QStandardItem("Timestamp"));
	setHorizontalHeaderItem(LOG_SEVERITY, new QStandardItem("Severity"));
	setHorizontalHeaderItem(LOG_UNIT, new QStandardItem("Unit"));
	setHorizontalHeaderItem(LOG_MESSAGE, new QStandardItem("Message"));
	setHorizontalHeaderItem(LOG_OBJECT, new QStandardItem("Object"));
}

LoggingItemModel::~LoggingItemModel(void)
{
}

void LoggingItemModel::clear(void)
{
	removeRows(0, rowCount());
}

Qt::ItemFlags LoggingItemModel::flags(const QModelIndex &oIndex) const
{
	Qt::ItemFlags fl = QStandardItemModel::flags(oIndex);
	if (!oIndex.isValid())
		return fl;

	// We want the modle to be editable, but it shouldn't be changeable, so we
	// will disable storing of edited data.
	fl |= Qt::ItemIsEditable;

	return fl;
}

bool LoggingItemModel::setData(QModelIndex const &oIndex, QVariant const &oValue, int nRole)
{
	if(nRole != Qt::EditRole)
		return QStandardItemModel::setData(oIndex, oValue, nRole);

	return false;
}

void LoggingItemModel::addItem(spt::logging::LoggingItem const &oItem)
{
	std::vector<spt::logging::LoggingItem> items;
	items.push_back(oItem);
	addItems(items);
}

void LoggingItemModel::setItems(std::vector<spt::logging::LoggingItem> const &oItems)
{
	clear();
	addItems(oItems);
}

void LoggingItemModel::addItems(std::vector<spt::logging::LoggingItem> const &oItems)
{
	if(oItems.size() == 0)
		return;

	int row = rowCount();
	int n = row+oItems.size();
	insertRows(row, n);

	int columns = columnCount();

	for(spt::logging::LoggingItem const &logitem : oItems)
	{
		for(int col = 0; col < columns; col++)
		{
			QStandardItem *item = new QStandardItem();
			spt::string::string_t t;

			switch(col)
			{
				case LOG_TIMESTAMP:
					t = spt::string::fromQt(QDateTime::fromMSecsSinceEpoch(logitem.getTimestamp()).toString("yyyy.MM.dd hh:mm:ss.zzz"));
				break;

				case LOG_SEVERITY:
					t = logitem.getTypeText();
				break;

				case LOG_MESSAGE:
					t = logitem.getMessage();
				break;

				case LOG_UNIT:
					t = logitem.getUnit();
				break;

				case LOG_OBJECT:
					t = logitem.getFile() + "(" + spt::string::toString(logitem.getLine()) + "):"+logitem.getMethod();
				break;
			}

			item->setText(spt::string::toQt(t));
			setItem(row, col, item);
		}
		row++;
	}
}

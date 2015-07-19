/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QDateTime>

#include "support_qt/logging/logging_item_model.h"
#include "support/logging/logging.h"

Q_DECLARE_METATYPE(supportlib::logging::LoggingItem)

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
	setHorizontalHeaderItem(LOG_TIMESTAMP, new QStandardItem(QString("Timestamp")));
	setHorizontalHeaderItem(LOG_SEVERITY, new QStandardItem(QString("Severity")));
	setHorizontalHeaderItem(LOG_UNIT, new QStandardItem(QString("Unit")));
	setHorizontalHeaderItem(LOG_MESSAGE, new QStandardItem(QString("Message")));
	setHorizontalHeaderItem(LOG_OBJECT, new QStandardItem(QString("Object")));
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

void LoggingItemModel::addItem(supportlib::logging::LoggingItem const &oItem)
{
	QList<supportlib::logging::LoggingItem> items;
	items.append(oItem);
	addItems(items);
}

void LoggingItemModel::setItems(QList<supportlib::logging::LoggingItem> const &oItems)
{
	clear();
	addItems(oItems);
}

void LoggingItemModel::addItems(QList<supportlib::logging::LoggingItem> const &oItems)
{
	if(oItems.count() == 0)
		return;

	int row = rowCount();
	int n = row+oItems.count();
	insertRows(row, n);

	int columns = columnCount();

	for(supportlib::logging::LoggingItem const &logitem : oItems)
	{
		for(int col = 0; col < columns; col++)
		{
			QStandardItem *item = new QStandardItem();
			QString t;

			switch(col)
			{
				case LOG_TIMESTAMP:
					t = QDateTime::fromMSecsSinceEpoch(logitem.getTimestamp()).toString("yyyy.MM.dd hh:mm:ss.zzz");
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
					t = logitem.getFile() + "(" + QString::number(logitem.getLine()) + "):"+logitem.getMethod();
				break;
			}

			item->setText(t);
			setItem(row, col, item);
		}
		row++;
	}
}

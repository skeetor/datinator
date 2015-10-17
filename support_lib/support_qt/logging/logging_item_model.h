/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef _LOGGING_ITEM_MODEL_H_INCLUDED
#define _LOGGING_ITEM_MODEL_H_INCLUDED

#include <QtGui/QStandardItemModel>
#include <vector>

#include "support_qt/support_qt_dll_api.h"
#include "support/logging/logging.h"

class SUPPORT_QT_DLL_EXPORT LoggingItemModel : public QStandardItemModel
{
public:
	LoggingItemModel(QObject *oParent = NULL);
	virtual ~LoggingItemModel(void);

	void addItem(spt::logging::LoggingItem const &oItem);
	void setItems(std::vector<spt::logging::LoggingItem> const &oItems);
	void addItems(std::vector<spt::logging::LoggingItem> const &oItems);

	bool setData(QModelIndex const &oIndex, QVariant const &oValue, int nRole = Qt::EditRole);
	void clear(void);

public: // @QStandardItemModel
	Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // _LOGGING_ITEM_MODEL_H_INCLUDED

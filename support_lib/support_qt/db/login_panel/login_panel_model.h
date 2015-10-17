/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef LOGIN_PANEL_MODEL_H_INCLUDED
#define LOGIN_PANEL_MODEL_H_INCLUDED

#include <QtGui/QStandardItemModel>

#include "support/unicode/unicode_types.h"
#include "support_qt/support_qt_dll_api.h"
#include "support/patterns/serialize.h"

class SUPPORT_QT_DLL_EXPORT LoginPanelModel
	: public QStandardItemModel
	, public Serialize<std::vector<spt::string::string_t>>
{
public:
	LoginPanelModel(void);
	virtual ~LoginPanelModel(void);

public: // @QStandardItemModel
	Qt::ItemFlags flags(const QModelIndex &index) const;

public: // @Serialize<T>
	virtual std::vector<spt::string::string_t> serialize(void);
	virtual bool deserialize(std::vector<spt::string::string_t> const &oList);
};

#endif // LOGIN_PANEL_MODEL_H_INCLUDED

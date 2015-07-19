/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef CSV_ITEM_MODEL_H_INCLUDED
#define CSV_ITEM_MODEL_H_INCLUDED

#include <QtCore/QList>
#include <QtGui/QStandardItemModel>

#include "datinator_types.h"
#include "plugin/plugin_dll_api.h"
#include "support/db/dbcolumn.h"

class PLUGIN_DLL_EXPORT SampleItemModel : public QStandardItemModel
{
public:
	SampleItemModel(void);
	virtual ~SampleItemModel(void);

public: // QStandardItemModel
	virtual Qt::ItemFlags flags(const QModelIndex &oIndex = QModelIndex()) const;

public:
	void clearRows(void);
	void showDatatypes(bool bShowDatatypes = true);
	void setColumns(QList<DatabaseColumn *> const &oColumns);
	void setRows(QList<QList<QString>> const &oSampleRows);

protected:
		void setRow(int nRow, QList<QString> const &oData);

private:
	bool mShowDatatypes;
	QList<DatabaseColumn *> mColumns;
};

#endif // CSV_ITEM_MODEL_H_INCLUDED

/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef COLUMN_EDITOR_MODEL_H_INCLUDED
#define COLUMN_EDITOR_MODEL_H_INCLUDED

#include <QtGui/QStandardItemModel>
#include <QtCore/QList>
#include "datinator_types.h"
#include "support/patterns/observer.h"
#include "support/db/dbcolumn.h"

class ColumnEditorModel
	:	public QStandardItemModel,
		public Dispatcher<bool, int, int>
{
protected:
public:
	ColumnEditorModel(QObject *oParent = NULL);
	virtual ~ColumnEditorModel(void);

	void setColumns(QList<DatabaseColumn *> const &oColumns);
	DatabaseColumn *columnItem(int nRow);

	/**
	 * The row listener will get updates whenever rows are inserted or deleted.
	 */
	void addRowListener(Listener<bool, int, int> *oListener);
	void removeRowListener(Listener<bool, int, int> *oListener);
	void clear(void);

public: // @QStandardItemModel
	Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool setData(QModelIndex const &oIndex, QVariant const &oValue, int nRole = Qt::EditRole) override;
	bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

protected:
	void notifyRowListeners(bool bInserted, int nRow, int nCount);
	void deleteRow(int nRow);
	void putItem(int nRow, DatabaseColumn *oItem);
	void updateName(int nRow, QString const &oName);
	void updateType(int nRow, QString const &oType);
};

#endif // COLUMN_EDITOR_MODEL_H_INCLUDED

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef SAMPLE_TABLE_VIEW_H_INCLUDED
#define SAMPLE_TABLE_VIEW_H_INCLUDED

#include <vector>
#include <QtWidgets/QTableView>

#include "plugin/plugin_dll_api.h"
#include "support_qt/models/delegates/datatype_combobox.h"
#include "plugin/gui/sample_table/sample_item_model.h"

class ContainerBase;

class PLUGIN_DLL_EXPORT SampleTableView : public QTableView
{
public:
	SampleTableView(QWidget *oParent = NULL);
	virtual ~SampleTableView(void);

	void setContainer(ContainerBase *oContainer);
	void setColumns(std::vector<DatabaseColumn *> const &oColumns);
	void clearRows(void);
	void setRows(std::vector<std::vector<StdString>> const &oSampleRows);
	void enableDatatypes(bool benableDatatypes = true);

protected:
	void updateWidgetView(void);

private:
	void init(void);

private:
	ContainerBase *mContainer;
	DatatypeCombobox<spt::db::DataType> *mDatatypeBox;
	bool mShowDatatypes;
	SampleItemModel mItemModel;
};

#endif // SAMPLE_TABLE_VIEW_H_INCLUDED

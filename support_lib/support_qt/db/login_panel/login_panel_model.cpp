/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QString>

#include "support_qt/db/login_panel/login_panel_model.h"
#include "support/db/database_login.h"
#include "support/helper/string.h"

Q_DECLARE_METATYPE(DatabaseLogin)

LoginPanelModel::LoginPanelModel(void)
:	QStandardItemModel(0, 1)
{
}

LoginPanelModel::~LoginPanelModel(void)
{
}

Qt::ItemFlags LoginPanelModel::flags(const QModelIndex &oIndex) const
{
	Qt::ItemFlags fl = QStandardItemModel::flags(oIndex);
	if (!oIndex.isValid())
		return fl;

	fl &= ~Qt::ItemIsEditable;

	return fl;
}

std::vector<spt::string::string_t> LoginPanelModel::serialize(void)
{
	std::vector<spt::string::string_t> logins;
	int n = rowCount();
	for(int i = 0; i < n; i++)
	{
		QVariant v = data(index(i, 0), Qt::UserRole);
		DatabaseLogin login = v.value<DatabaseLogin>();
		spt::string::string_t serialized = login.serialize();
		logins.push_back(serialized);
	}

	return logins;
}

bool LoginPanelModel::deserialize(std::vector<spt::string::string_t> const &oLogins)
{
	removeRows(0, rowCount());
	if(oLogins.size() == 0)
		return true;

	insertRows(0, oLogins.size());
	int i = 0;
	QStandardItem *item;
	for(spt::string::string_t const &serialized : oLogins)
	{
		DatabaseLogin login(serialized);
		QVariant v;
		v.setValue(login);
		spt::string::string_t t = login.getUser()+ "@"+login.getDatabase();
		item = new QStandardItem();
		item->setText(spt::string::toQt(t));
		item->setData(v, Qt::UserRole);
		setItem(i, 0, item);
		i++;
	}

	return true;
}

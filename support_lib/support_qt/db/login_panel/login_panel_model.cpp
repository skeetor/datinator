/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include "support_qt/db/login_panel/login_panel_model.h"
#include "support/db/database_login.h"

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

QList<QString> LoginPanelModel::serialize(void)
{
	QList<QString> logins;
	int n = rowCount();
	for(int i = 0; i < n; i++)
	{
		QVariant v = data(index(i, 0), Qt::UserRole);
		DatabaseLogin login = v.value<DatabaseLogin>();
		QString serialized = supportlib::string::StringTToQtString(login.serialize());
		logins.push_back(serialized);
	}

	return logins;
}

bool LoginPanelModel::deserialize(QList<QString> const &oLogins)
{
	removeRows(0, rowCount());
	if(oLogins.size() == 0)
		return true;

	insertRows(0, oLogins.size());
	int i = 0;
	QStandardItem *item;
	for(QString const &serialized : oLogins)
	{
		DatabaseLogin login(supportlib::string::QtStringToStringT(serialized));
		QVariant v;
		v.setValue(login);
		QString t = supportlib::string::StringTToQtString(login.getUser())
			+ "@"+supportlib::string::StringTToQtString(login.getDatabase())
		;
		item = new QStandardItem();
		item->setText(t);
		item->setData(v, Qt::UserRole);
		setItem(i, 0, item);
		i++;
	}

	return true;
}

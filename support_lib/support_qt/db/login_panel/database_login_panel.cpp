/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QString>

#include "support/support_defs.h"
#include "support/helper/string.h"

#include "support_qt/helper/gui_helper.h"
#include "support_qt/db/login_panel/database_login_panel_gui.moc"

Q_DECLARE_METATYPE(DatabaseLogin)

LoginPanelModel *gModel = NULL;

DatabaseLoginPanel::DatabaseLoginPanel(bool bUseSharedModel, QWidget *oParent)
: QFrame(oParent)
{
	mCurrent = -1;
	if(bUseSharedModel)
	{
		if(gModel == NULL)
			gModel = new LoginPanelModel();

		mModel = gModel;
	}
	else
		mModel = new LoginPanelModel();

	mGUI = new Ui::DatabaseLoginGUI();
	mGUI->setupUi(this);
	spt::gui::setButtonIcon(spt::image::button_add, spt::image::button_add_length, mGUI->mSaveBtn, "Save connection");
	spt::gui::setButtonIcon(spt::image::button_delete, spt::image::button_delete_length, mGUI->mRemoveBtn, "Remove connection");
	spt::gui::setButtonIcon(spt::image::button_up, spt::image::button_up_length, mGUI->mMoveUpBtn, "Move connection up");
	spt::gui::setButtonIcon(spt::image::button_down, spt::image::button_down_length, mGUI->mMoveDownBtn, "Move connection down");

	mGUI->mLoginListView->setModel(mModel);
	mGUI->mLoginListView->setSelectionMode(QAbstractItemView::SingleSelection);
}

DatabaseLoginPanel::~DatabaseLoginPanel(void)
{
}

void DatabaseLoginPanel::addLoginListener(Listener<LoginEvent> *oListener)
{
	Dispatcher<LoginEvent>::addListener(oListener);
}

void DatabaseLoginPanel::removeLoginListener(Listener<LoginEvent> *oListener)
{
	Dispatcher<LoginEvent>::removeListener(oListener);
}

void DatabaseLoginPanel::notifyLoginListeners(LoginEvent const &oLoginEvent)
{
	Dispatcher<LoginEvent>::notify(oLoginEvent);
}

LoginPanelModel *DatabaseLoginPanel::getModel(void)
{
	return mModel;
}

void DatabaseLoginPanel::clear(void)
{
	mCurrent = -1;
	mGUI->mUserTxt->setText("");
	mGUI->mPasswordTxt->setText("");
	mGUI->mDatabaseTxt->setText("");
	mGUI->mHostTxt->setText("");
	mGUI->mSaveHostBox->setCheckState(Qt::CheckState::Unchecked);
	mGUI->mLoginListView->clearSelection();
}

void DatabaseLoginPanel::setLogin(DatabaseLogin const &oLogin)
{
	mGUI->mUserTxt->setText(spt::string::toQt(oLogin.getUser()));
	mGUI->mPasswordTxt->setText(spt::string::toQt(oLogin.getClearPassword()));
	mGUI->mDatabaseTxt->setText(spt::string::toQt(oLogin.getDatabase()));
	mGUI->mHostTxt->setText(spt::string::toQt(oLogin.getHost()));
	if(oLogin.requiresSaveHost())
		mGUI->mSaveHostBox->setCheckState(Qt::CheckState::Checked);
	else
		mGUI->mSaveHostBox->setCheckState(Qt::CheckState::Unchecked);
}

DatabaseLogin DatabaseLoginPanel::getLogin(void) const
{
	DatabaseLogin login;

	login.setUser(spt::string::fromQt(mGUI->mUserTxt->text()));
	login.setClearPassword(spt::string::fromQt(mGUI->mPasswordTxt->text()));
	login.setDatabase(spt::string::fromQt(mGUI->mDatabaseTxt->text()));
	login.setHost(spt::string::fromQt(mGUI->mHostTxt->text()));
	if(mGUI->mSaveHostBox->checkState() == Qt::CheckState::Checked)
		login.saveHost(true);
	else
		login.saveHost(false);

	return login;
}

void DatabaseLoginPanel::setReadOnly(bool bReadOnly)
{
	mGUI->mSaveBtn->setEnabled(!bReadOnly);
	mGUI->mRemoveBtn->setEnabled(!bReadOnly);
	mGUI->mMoveUpBtn->setEnabled(!bReadOnly);
	mGUI->mMoveDownBtn->setEnabled(!bReadOnly);
	mGUI->mSaveHostBox->setEnabled(!bReadOnly);
	spt::gui::setEditable(mGUI->mHostTxt, !bReadOnly);
	spt::gui::setEditable(mGUI->mUserTxt, !bReadOnly);
	spt::gui::setEditable(mGUI->mPasswordTxt, !bReadOnly);
	spt::gui::setEditable(mGUI->mDatabaseTxt, !bReadOnly);
}

bool DatabaseLoginPanel::validateUser(spt::string::string_t const &oUser) const
{
	if(oUser.length() == 0)
		return false;

	return true;
}

bool DatabaseLoginPanel::validatePassword(spt::string::string_t const &oPassword) const
{
	UNUSED(oPassword);

	return true;
}

bool DatabaseLoginPanel::validateDatabase(spt::string::string_t const &oDatabase) const
{
	if(oDatabase.length() == 0)
		return false;

	return true;
}

bool DatabaseLoginPanel::validateHost(spt::string::string_t const &oHost) const
{
	if(oHost.length() == 0)
		return false;

	return true;
}

void DatabaseLoginPanel::addItem(DatabaseLogin const &oLogin, bool bNotify)
{
	int i = find(oLogin);
	QStandardItem *item = NULL;

	QVariant v;
	v.setValue(oLogin);

	if(i == -1)
	{
		item = new QStandardItem();
		i = mModel->rowCount();
		mModel->insertRow(i, item);
	}
	else
		item = mModel->takeItem(i);

	auto t = spt::string::toQt(oLogin.getUser()+ "@"+oLogin.getDatabase());
	item->setText(t);
	item->setData(v, Qt::UserRole);
	mModel->setItem(i, 0, item);
	if(bNotify)
	{
		QModelIndex ind = mModel->index(i, 0);
		mGUI->mLoginListView->setCurrentIndex(ind);
		onItemClicked(ind);
	}
}

void DatabaseLoginPanel::onSave(void)
{
	DatabaseLogin login = getLogin();
	addItem(login, true);
}

void DatabaseLoginPanel::onRemove(void)
{
	if(mCurrent == -1)
		return;

	QStandardItem *cur = mModel->takeItem(mCurrent);
	QModelIndex ind = mModel->index(mCurrent, 0);
	mModel->removeRow(mCurrent);
	onItemClicked(ind);
	delete cur;
	clear();
}

void DatabaseLoginPanel::onItemClicked(QModelIndex const &oModelIndex)
{
	if(!oModelIndex.isValid())
		return;

	DatabaseLogin login;
	bool active;
	if(!mGUI->mLoginListView->selectionModel()->isRowSelected(oModelIndex.row(), QModelIndex()))
	{
		mCurrent = -1;
		clear();
		active = false;
	}
	else
	{
		mCurrent = oModelIndex.row();
		QVariant v = mModel->data(oModelIndex, Qt::UserRole);
		login = v.value<DatabaseLogin>();
		setLogin(login);
		active = true;
	}

	LoginEvent event(this, login, active);
	notifyLoginListeners(event);
}

void DatabaseLoginPanel::onMoveUp(void)
{
	if(!(mCurrent > 0 && mModel->rowCount() > 1))
		return;

	int i = mCurrent;
	QStandardItem *cur = mModel->takeItem(i);
	QStandardItem *next = mModel->takeItem(i-1);
	mModel->setItem(i, 0, next);
	mModel->setItem(i-1, 0, cur);
	QModelIndex ind = mModel->index(i-1, 0);
	mCurrent = i-1;
	mGUI->mLoginListView->setCurrentIndex(ind);
}

void DatabaseLoginPanel::onMoveDown(void)
{
	int n = mModel->rowCount();
	if(mCurrent == -1 || mCurrent >= n)
		return;

	int i = mCurrent;
	QStandardItem *cur = mModel->takeItem(i);
	QStandardItem *next = mModel->takeItem(i+1);
	mModel->setItem(i, 0, next);
	mModel->setItem(i+1, 0, cur);
	QModelIndex ind = mModel->index(i+1, 0);
	mCurrent = i+1;
	mGUI->mLoginListView->setCurrentIndex(ind);
}

int DatabaseLoginPanel::find(DatabaseLogin const &oLogin) const
{
	int n = mModel->rowCount();
	for(int i = 0; i < n; i++)
	{
		QVariant v = mModel->data(mModel->index(i, 0), Qt::UserRole);
		DatabaseLogin login = v.value<DatabaseLogin>();
		if(oLogin == login)
			return i;
	}

	return -1;
}

bool DatabaseLoginPanel::select(spt::string::string_t const &oDatabase, spt::string::string_t const &oUser, spt::string::string_t const &oHost, bool bNotify)
{
	DatabaseLogin l;

	l.setDatabase(oDatabase);
	l.setUser(oUser);
	l.setHost(oHost);

	return select(l, bNotify);
}

bool DatabaseLoginPanel::select(DatabaseLogin const &oLogin, bool bNotify)
{
	int i = find(oLogin);
	if(i == -1)
		return false;

	QModelIndex ind = mModel->index(i, 0);
	mCurrent = i;
	mGUI->mLoginListView->setCurrentIndex(ind);
	DatabaseLogin l = getLogin(i);
	setLogin(l);
	if(bNotify)
		onItemClicked(ind);

	return true;
}

DatabaseLogin DatabaseLoginPanel::getLogin(int nIndex)
{
	DatabaseLogin l;

	if(nIndex < 0 || nIndex > mModel->rowCount())
		return l;

	QVariant v = mModel->data(mModel->index(nIndex, 0), Qt::UserRole);
	l = v.value<DatabaseLogin>();

	return l;
}

/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeySequence>

#include "support/helper/string.h"

#include "plugin/sql/gui/sql_preview_gui.moc"

Q_DECLARE_METATYPE(StdString)

SQLPreviewPanel::SQLPreviewPanel(QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::SQLPreviewGUI();
	mGUI->setupUi(this);

	// Install the keyboard handler for allowing us to
	// use CTRL+Enter/Return as a shortcut to execute
	// the query.
	QKeySequence qse = QKeySequence(Qt::CTRL + Qt::Key_Enter);
	mActionEnter = new QAction(this);
	mActionEnter->setShortcut(qse);
	connect(mActionEnter, SIGNAL(triggered()), this, SLOT(onExecute()));
	mGUI->mSQLTxt->addAction(mActionEnter);

	QKeySequence qsr = QKeySequence(Qt::CTRL + Qt::Key_Return);
	mActionReturn = new QAction(this);
	mActionReturn->setShortcut(qsr);
	connect(mActionReturn, SIGNAL(triggered()), this, SLOT(onExecute()));
	mGUI->mSQLTxt->addAction(mActionReturn);
}

SQLPreviewPanel::~SQLPreviewPanel(void)
{
}

void SQLPreviewPanel::addActionListener(Listener<SQLPreview::ActionEvent, QVariant, int> *oListener)
{
	Dispatcher<SQLPreview::ActionEvent, QVariant, int>::addListener(oListener);
}

void SQLPreviewPanel::removeActionListener(Listener<SQLPreview::ActionEvent, QVariant, int> *oListener)
{
	Dispatcher<SQLPreview::ActionEvent, QVariant, int>::removeListener(oListener);
}

void SQLPreviewPanel::notifyActionListener(SQLPreview::ActionEvent nEvent, QVariant oData, int nRows)
{
	Dispatcher<SQLPreview::ActionEvent, QVariant, int>::notify(nEvent, oData, nRows);
}

void SQLPreviewPanel::onSQL(bool bState)
{
	mGUI->mSQLTxt->setEnabled(bState);
	notifyActionListener(SQLPreview::ActionEvent::EVENT_SQL_TOGGLE, bState);
}

void SQLPreviewPanel::onExecute(void)
{
	// We also send an empty query, because the view may want to
	// update to an empty display.
	StdString s = spt::string::fromQt(mGUI->mSQLTxt->toPlainText());
	if(mGUI->mSQLBox->isChecked())
	{
		int rows = mGUI->mPreviewSpin->value();
		notifyActionListener(SQLPreview::ActionEvent::EVENT_SQL_EXECUTE, spt::string::toQt(s), rows);
	}
	else
		notifyActionListener(SQLPreview::ActionEvent::EVENT_SQL_TOGGLE, mGUI->mSQLTxt->isEnabled());
}

StdString SQLPreviewPanel::getQuery(void)
{
	return spt::string::fromQt(mGUI->mSQLTxt->toPlainText());
}

void SQLPreviewPanel::setQuery(StdString const &oQuery, bool bNotify)
{
	mGUI->mSQLTxt->setText(spt::string::toQt(oQuery));
	if(bNotify)
	{
		int rows = mGUI->mPreviewSpin->value();
		notifyActionListener(SQLPreview::ActionEvent::EVENT_SQL_EXECUTE, spt::string::toQt(oQuery), rows);
	}
}

void SQLPreviewPanel::setPreviewLimit(int nCount)
{
	mGUI->mPreviewSpin->setValue(nCount);
}

int SQLPreviewPanel::getPreviewLimit(void)
{
	return mGUI->mPreviewSpin->value();
}

void SQLPreviewPanel::setPreview(std::vector<DatabaseColumn *> const &oColumns, std::vector<std::vector<StdString>> const &oRows)
{
	mGUI->mSQLView->setColumns(oColumns);
	mGUI->mSQLView->setRows(oRows);
}

bool SQLPreviewPanel::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	auto k = spt::string::toQt(oKey);

	setPreviewLimit(oProfile.value(k+"_preview_rows", "10").toInt());
	mGUI->mSQLBox->setChecked(oProfile.value(k+"_sql_state", "false").toBool());
	bool sql = mGUI->mSQLBox->isChecked();
	bool notifyFlag = sql;
	mGUI->mSQLTxt->setEnabled(sql);
	setQuery(spt::string::fromQt(oProfile.value(k+"_sql", "").toString()), notifyFlag);

	return true;
}

void SQLPreviewPanel::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	auto k = spt::string::toQt(oKey);

	oProfile.setValue(k+"_preview_rows", getPreviewLimit());
	oProfile.setValue(k+"_sql_state", mGUI->mSQLBox->isChecked());
	oProfile.setValue(k+"_sql", spt::string::toQt(getQuery()));
}

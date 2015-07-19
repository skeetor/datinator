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

#include "plugin/sql/gui/sql_preview_gui.moc"

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
	QString s = mGUI->mSQLTxt->toPlainText();
	if(mGUI->mSQLBox->isChecked())
	{
		int rows = mGUI->mPreviewSpin->value();
		notifyActionListener(SQLPreview::ActionEvent::EVENT_SQL_EXECUTE, s, rows);
	}
	else
		notifyActionListener(SQLPreview::ActionEvent::EVENT_SQL_TOGGLE, mGUI->mSQLTxt->isEnabled());
}

QString SQLPreviewPanel::getQuery(void)
{
	return mGUI->mSQLTxt->toPlainText();
}

void SQLPreviewPanel::setQuery(QString const &oQuery, bool bNotify)
{
	mGUI->mSQLTxt->setText(oQuery);
	if(bNotify)
	{
		int rows = mGUI->mPreviewSpin->value();
		notifyActionListener(SQLPreview::ActionEvent::EVENT_SQL_EXECUTE, oQuery, rows);
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

void SQLPreviewPanel::setPreview(QList<DatabaseColumn *> const &oColumns, QList<QList<QString>> const &oRows)
{
	mGUI->mSQLView->setColumns(oColumns);
	mGUI->mSQLView->setRows(oRows);
}

bool SQLPreviewPanel::loadProfile(QSettings &oProfile, QString const &oKey)
{
	setPreviewLimit(oProfile.value(oKey+"_preview_rows", "10").toInt());
	mGUI->mSQLBox->setChecked(oProfile.value(oKey+"_sql_state", "false").toBool());
	bool sql = mGUI->mSQLBox->isChecked();
	bool notifyFlag = sql;
	mGUI->mSQLTxt->setEnabled(sql);
	setQuery(oProfile.value(oKey+"_sql", "").toString(), notifyFlag);

	return true;
}

void SQLPreviewPanel::saveProfile(QSettings &oProfile, QString const &oKey)
{
	oProfile.setValue(oKey+"_preview_rows", getPreviewLimit());
	oProfile.setValue(oKey+"_sql_state", mGUI->mSQLBox->isChecked());
	oProfile.setValue(oKey+"_sql", getQuery());
}

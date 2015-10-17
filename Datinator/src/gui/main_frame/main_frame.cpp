/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <utility>

#include <QtCore/QVariant>
#include <QtCore/QModelIndex>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>

#include <support/helper/string.h>

#include <support_qt/helper/signal_blocker.h>
#include <support_qt/helper/gui_helper.h>

#include "plugins/idata_container_reader.h"
#include "plugins/idata_container_writer.h"

#include "plugins/plugin/plugin_manager.h"

#include "gui/mapping/column_mapping_item.h"
#include "gui/main_frame/main_frame_gui.moc"
#include "gui/main_frame/most_recent.h"
#include "plugin/gui/progress.h"

#define RECENT_PROFILE_TAG  "RECENT_PROFILE"

#warning TODO: Save the name of a UUID as well to display a more meaningfull error message when loading/saving profiles.

Q_DECLARE_METATYPE(IDataContainerReader *)
Q_DECLARE_METATYPE(IDataContainerWriter *)

MainFrame::MainFrame(MostRecent *oRecentSource, MostRecent *oRecentTarget, MostRecent *oRecentProfile, QWidget *oParent)
:	QFrame(oParent),
	mRecentSource(oRecentSource),
	mRecentTarget(oRecentTarget),
	mRecentProfile(oRecentProfile),
	mReaderConfigListener(this, oRecentSource),
	mWriterConfigListener(this, oRecentTarget)
{
	mRecentProfileTag = RECENT_PROFILE_TAG;
	mCurSourcePanel = NULL;
	mCurTargetPanel = NULL;
	Progress::setParent(this);

	mRecentSource->addMenuItemListener(this);
	mRecentTarget->addMenuItemListener(this);
	mRecentProfile->addMenuItemListener(this);
	mRecentProfile->setMenuReference(mRecentProfileTag);

	mGUI = new Ui::MainFrameGUI();
	mGUI->setupUi(this);
	mGUI->mColumnSpin->setKeyboardTracking(false);
	mGUI->mMappingTableView->addRowcountListener(this);

	StdString p = spt::string::fromQt(QApplication::applicationDirPath());
	mPluginManager = new PluginManager();
	mPluginManager->addPath(p);

	spt::gui::setButtonIcon(spt::image::button_add, spt::image::button_add_length, mGUI->mInsertBtn, "Add row(s)");
	spt::gui::setButtonIcon(spt::image::button_delete, spt::image::button_delete_length, mGUI->mRemoveBtn, "Remove row(s)");
	spt::gui::setButtonIcon(spt::image::button_down, spt::image::button_down_length, mGUI->mDownBtn, "Move down");
	spt::gui::setButtonIcon(spt::image::button_up, spt::image::button_up_length, mGUI->mUpBtn, "Move up");
	spt::gui::setButtonIcon(p+"/res/Load.png", mGUI->mLoadBtn, "Load");
	spt::gui::setButtonIcon(p+"/res/Save.png", mGUI->mSaveBtn, "Save");

	QVBoxLayout *l = new QVBoxLayout(this);
	l->setContentsMargins(0, 0, 0, 0);
	mGUI->mSourcePanel->setLayout(l);

	l = new QVBoxLayout(this);
	l->setContentsMargins(0, 0, 0, 0);
	mGUI->mTargetPanel->setLayout(l);

	mRestorable.addWidget("frame", this);
	mRestorable.addSplitter("right_splitter", mGUI->mMainSplitter);
	mRestorable.addSplitter("left_splitter", mGUI->mLeftSplitter);
	mRestorable.addTable("mapping", mGUI->mMappingTableView);

	initReaders();
	initWriters();

	updateGUIElements();

	onSourceSelected(0);
	onTargetSelected(0);
}

MainFrame::~MainFrame(void)
{
}

void MainFrame::reloadPlugins(void)
{
	Progress prg("Loading plugins", "Initializing ...");

	mPluginManager->reload();
	initReaders();
	initWriters();

	updateGUIElements();

	onSourceSelected(0);
	onTargetSelected(0);

	std::vector<std::pair<StdString, std::vector<std::pair<StdString, StdString>>>> duplicates = mPluginManager->findDuplicates();
	for(std::pair<StdString, std::vector<std::pair<StdString, StdString>>> const &duplicate : duplicates)
	{
		StdString uuid = duplicate.first;
		std::vector<std::pair<StdString, StdString>> dups = duplicate.second;
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Plugins", "The "+uuid+" has to be unique but exists in multiple plugins:");
		for(std::pair<StdString, StdString> const &plugin : dups)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Plugins", "DLL: "+plugin.first+" Containername: "+plugin.second);
		}
	}
}

void MainFrame::updateGUIElements(void)
{
	bool enable = true;
	if(getTargetBox()->count() == 0 || getSourceBox()->count() == 0)
		enable = false;

	mGUI->mColumnSpin->setEnabled(enable);
	mGUI->mCopyBtn->setEnabled(enable);
	mGUI->mResetBtn->setEnabled(enable);
    mGUI->mInsertBtn->setEnabled(enable);
    mGUI->mRemoveBtn->setEnabled(enable);
    mGUI->mUpBtn->setEnabled(enable);
    mGUI->mDownBtn->setEnabled(enable);
    mGUI->mLoadBtn->setEnabled(enable);
    mGUI->mSaveBtn->setEnabled(enable);
    mGUI->mTruncateBtn->setEnabled(enable);
    mGUI->mAppendBtn->setEnabled(enable);
}

void MainFrame::writeMostRecentItems(QSettings &oPropertyfile, StdString const &oUUID, MostRecent *oMostRecent)
{
	int i = -1;
	auto kv = spt::string::toQt("main/"+oUUID+"_");
	std::vector<MostRecent::ActionItem> il = oMostRecent->getItems(oUUID);

	oPropertyfile.setValue(kv+"items", il.size());

	for(MostRecent::ActionItem &ai : il)
	{
		i++;
		auto k = kv+spt::string::toQt(spt::string::toString(i))+"_";

		oPropertyfile.setValue(k+"connect", spt::string::toQt(ai.mConnectString));
		oPropertyfile.setValue(k+"text", spt::string::toQt(ai.mText));
	}
}

void MainFrame::readMostRecentItems(QSettings &oPropertyfile, StdString const &oUUID, MostRecent *oMostRecent)
{
	auto kv = spt::string::toQt("main/"+oUUID+"_");
	int n = oPropertyfile.value(kv+"items").toInt();

	// The insertion into the most recent list, has to be reversed
	// so that the most recent item is added last.
	for(int i = n-1; i >= 0; i--)
	{
		auto k = kv+spt::string::toQt(spt::string::toString(i))+"_";

		StdString con = spt::string::fromQt(oPropertyfile.value(k+"connect").toString());
		StdString txt = spt::string::fromQt(oPropertyfile.value(k+"text").toString());
		oMostRecent->addItem(oUUID, con, txt);
	}
}

void MainFrame::store(QSettings &oPropertyfile)
{
	mRestorable.store(oPropertyfile, "main/");

	oPropertyfile.setValue("main/most_recent_profiles", mRecentProfile->getMaxItems());
	writeMostRecentItems(oPropertyfile, mRecentProfileTag, mRecentProfile);

	oPropertyfile.setValue("main/most_recent_max", mRecentSource->getMaxItems());

	QComboBox *b = getSourceBox();
    int n = b->count();
    for(int i = 0; i < n; i++)
	{
		QVariant v = b->itemData(i);
		IDataContainerReader *idc = v.value<IDataContainerReader *>();
		writeMostRecentItems(oPropertyfile, idc->getContainerUUID(), mRecentSource);
		idc->store(oPropertyfile, "src_");
	}
	b = getTargetBox();
    n = b->count();
    for(int i = 0; i < n; i++)
	{
		QVariant v = b->itemData(i);
		IDataContainerWriter *idc = v.value<IDataContainerWriter *>();
		writeMostRecentItems(oPropertyfile, idc->getContainerUUID(), mRecentTarget);
		idc->store(oPropertyfile, "tgt_");
	}
}

void MainFrame::restore(QSettings &oPropertyfile)
{
	mRestorable.restore(oPropertyfile, "main/");
	mRestorable.restore(oPropertyfile, "main/");
	int n = oPropertyfile.value("main/most_recent_max").toInt();
	mRecentSource->setMaxItems(n);
	mRecentTarget->setMaxItems(n);

	n = oPropertyfile.value("main/most_recent_profiles").toInt();
	mRecentProfile->setMaxItems(n);
	readMostRecentItems(oPropertyfile, mRecentProfileTag, mRecentProfile);

	QComboBox *b = getSourceBox();
    n = b->count();
    for(int i = 0; i < n; i++)
	{
		QVariant v = b->itemData(i);
		IDataContainerReader *idc = v.value<IDataContainerReader *>();
		readMostRecentItems(oPropertyfile, idc->getContainerUUID(), mRecentSource);
		idc->restore(oPropertyfile, "src_");
	}

	b = getTargetBox();
    n = b->count();
    for(int i = 0; i < n; i++)
	{
		QVariant v = b->itemData(i);
		IDataContainerWriter *idc = v.value<IDataContainerWriter *>();
		readMostRecentItems(oPropertyfile, idc->getContainerUUID(), mRecentTarget);
		idc->restore(oPropertyfile, "tgt_");
	}
}

void MainFrame::initReaders(void)
{
	QComboBox *box = getSourceBox();
	box->clear();

	std::vector<IDataContainerReader *> l = mPluginManager->getReaders(this);
	for(IDataContainerReader *r : l)
		initReaderContainerBox(box, r);

	if(l.size() > 0)
		box->setCurrentIndex(0);
}

void MainFrame::initWriters(void)
{
	QComboBox *box = getTargetBox();
	box->clear();

	std::vector<IDataContainerWriter *> l = mPluginManager->getWriters(this);
	for(IDataContainerWriter *w : l)
		initWriterContainerBox(box, w);

	if(l.size() > 0)
		box->setCurrentIndex(0);
}

void MainFrame::initReaderContainerBox(QComboBox *oBox, IDataContainerReader *oReader)
{
	QVariant v;
	v.setValue(oReader);
	oBox->addItem(spt::string::toQt(oReader->getContainername()), v);
	oReader->addColumnListener(&mReaderConfigListener);
	oReader->addSelectorListener(&mReaderConfigListener);

	mRecentSource->addMenu(oReader->getContainerUUID(), oReader->getContainername());
}

void MainFrame::initWriterContainerBox(QComboBox *oBox, IDataContainerWriter *oWriter)
{
	QVariant v;
	v.setValue(oWriter);
	oBox->addItem(spt::string::toQt(oWriter->getContainername()), v);
	oWriter->addColumnListener(&mWriterConfigListener);
	oWriter->addSelectorListener(&mWriterConfigListener);
	mRecentTarget->addMenu(oWriter->getContainerUUID(), oWriter->getContainername());
}

QComboBox *MainFrame::getSourceBox(void)
{
	return mGUI->mSourceBox;
}

QComboBox *MainFrame::getTargetBox(void)
{
	return mGUI->mTargetBox;
}

QWidget *MainFrame::getSourcePanel(void)
{
	return mGUI->mSourcePanel;
}

QWidget *MainFrame::getTargetPanel(void)
{
	return mGUI->mTargetPanel;
}

void MainFrame::onCopy(void)
{
	IDataContainerReader *sc = getCurrentSourceContainer();
	IDataContainerWriter *tc = getCurrentTargetContainer();
	performCopy(sc, tc);
}

void MainFrame::onReset(void)
{
	IDataContainerWriter *w = getCurrentTargetContainer();
	IDataContainerReader *r = getCurrentSourceContainer();

	if(!w || !r)
		return;

    setTargetColumns(w->getColumns());
	setSourceColumns(r->getColumns());
	mGUI->mMappingTableView->clearSelection();
}

void MainFrame::onEdit(void)
{
	IDataContainerWriter *w = getCurrentTargetContainer();
	if(!w)
		return;

	ColumnMappingView *v = mGUI->mMappingTableView;
	std::vector<DatabaseColumn *> vl = v->getTargetColumns();
	mColumnEditor.setColumns(vl);
	mColumnEditor.exec();
	if(mColumnEditor.result() == QDialog::Accepted)
		setTargetColumns(mColumnEditor.getColumns());
}

void MainFrame::onRemove(void)
{
	mGUI->mMappingTableView->removeSelected();
	int n = mGUI->mMappingTableView->rowCount();
	QSpinBox *sp = mGUI->mColumnSpin;
	SignalBlocker blocker(sp);
	sp->setValue(n);
}

void MainFrame::onUp(void)
{
	mGUI->mMappingTableView->moveUp();
}

void MainFrame::onDown(void)
{
	mGUI->mMappingTableView->moveDown();
}

void MainFrame::onLoad(void)
{
	loadProfile();
}

void MainFrame::onSave(void)
{
	saveProfile();
}

void MainFrame::handleNotification(Dispatcher<StdString, StdString, StdString> *oSource, StdString oContainerId, StdString oConnectString, StdString oSelectorId)
{
	UNUSED(oSource);

	IDataContainer *dc = NULL;

	if(oContainerId == mRecentProfileTag)
	{
		loadProfile(&oSelectorId);
		return;
	}

	int i = findContainer(getTargetBox(), oContainerId);
	if(i != -1)
	{
		getTargetBox()->setCurrentIndex(i);
		onTargetSelected(i);
		dc = getCurrentTargetContainer();
	}
	else
	{
		i = findContainer(getSourceBox(), oContainerId);
		if(i != -1)
		{
			getSourceBox()->setCurrentIndex(i);
			onSourceSelected(i);
			dc = getCurrentSourceContainer();
		}
	}

	if(dc)
	{
		dc->connect(oConnectString);
		dc->setSelector(oSelectorId);
	}
}

void MainFrame::handleNotification(Dispatcher<int> *oSource, int nRows)
{
	UNUSED(oSource);

	setRowcount(nRows);
}

void MainFrame::setRowcount(int nRows)
{
	if(getTargetBox()->count() == 0 || getSourceBox()->count() == 0)
		return;

	if(mGUI->mColumnSpin->value() != nRows)
		mGUI->mColumnSpin->setValue(nRows);
}

int MainFrame::getRowcount(void)
{
	return mGUI->mColumnSpin->value();
}

IDataContainerReader *MainFrame::getCurrentSourceContainer(void)
{
	QVariant v = getSourceBox()->currentData();
	return v.value<IDataContainerReader *>();
}

IDataContainerWriter *MainFrame::getCurrentTargetContainer(void)
{
	QVariant v = getTargetBox()->currentData();
	return v.value<IDataContainerWriter *>();
}

int MainFrame::findContainer(QComboBox *oBox, StdString const &oUUID)
{
	int n = oBox->count();
	for(int i = 0; i < n; i++)
	{
		QVariant v = oBox->itemData(i);
		IDataContainerReader *r = v.value<IDataContainerReader *>();
		IDataContainer *dc = NULL;
		if(r)
			dc = r;
		else
		{
			IDataContainerWriter *w = v.value<IDataContainerWriter *>();
			if(w)
				dc = w;
		}

		if(dc->getContainerUUID() == oUUID)
			return i;
	}

	return -1;
}

void MainFrame::onSourceSelected(int nIndex)
{
	UNUSED(nIndex);

	IDataContainerReader *s = getCurrentSourceContainer();
	if(!s)
		return;

	Progress p("Source selected, updating Target ...", "Loading columns ...");
	updateConfigPanel(s, mCurSourcePanel, mGUI->mSourcePanel);
	setSourceColumns(s->getColumns());
}

void MainFrame::onTargetSelected(int nIndex)
{
	UNUSED(nIndex);

	IDataContainerWriter *t = getCurrentTargetContainer();
	if(!t)
		return;
	Progress p("Target selected, updating Target ...", "Loading columns ...");

	bool modifyColumns = t->canModifyColumns();
	mGUI->mMappingTableView->setReadOnly(!modifyColumns);
	mGUI->mColumnSpin->setReadOnly(!modifyColumns);
	mGUI->mColumnSpin->setDisabled(!modifyColumns);

	updateConfigPanel(t, mCurTargetPanel, mGUI->mTargetPanel);
	bool enabled = t->canTruncate();
	mGUI->mTruncateBtn->setEnabled(enabled);
    mGUI->mAppendBtn->setEnabled(enabled);
    if(t->defaultTruncate())
		mGUI->mTruncateBtn->setChecked(true);
	else
		mGUI->mAppendBtn->setChecked(true);

    setTargetColumns(t->getColumns());
}

void MainFrame::updateConfigPanel(IDataContainer *oContainer, QWidget *&oCurrentPanel, QWidget *oMainPanel)
{
	QWidget *old = oCurrentPanel;
	oCurrentPanel = NULL;

	QWidget *cp = oContainer->getConfigPanel();
	if(cp == NULL)
		goto Quit;

	if(old == NULL || old != cp)
	{
		QLayout *l = oMainPanel->layout();
		if(old)
			l->removeWidget(old);

		cp->show();
		l->addWidget(cp);
	}

	oCurrentPanel = cp;

Quit:
	if(old != NULL && old != cp)
		old->hide();
}

void MainFrame::setSourceColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	mGUI->mMappingTableView->setSourceColumns(oColumns);
}

void MainFrame::setTargetColumns(std::vector<DatabaseColumn *> const &oColumns)
{
	mGUI->mMappingTableView->setTargetColumns(oColumns);
}

std::vector<ColumnMappingItem> MainFrame::prepareItems(std::vector<ColumnMappingItem> const &oItems)
{
	std::vector<ColumnMappingItem> mappings;

	for(ColumnMappingItem const &mi : oItems)
	{
		if(mi.getTargetColumn() == NULL)
			continue;

		mappings.push_back(mi);
	}

	return mappings;
}

bool MainFrame::performCopy(IDataContainerReader *oReader, IDataContainerWriter *oWriter)
{
	Progress prg("Copying ...", "Initializing...");
	if(!oReader || !oWriter)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "No reader or writer selected!");
		return false;
	}

	int n = 0;
	bool ok = true;
	bool canceled = false;
	try
	{
		if(mGUI->mTruncateBtn->isEnabled() && mGUI->mTruncateBtn->isChecked())
			oWriter->setTruncateMode(true);
		else
			oWriter->setTruncateMode(false);

		if(!oReader->connect())
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "Unable to connect to source.");
			return false;
		}

		if(!oWriter->connect())
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "Unable to connect to target.");
			return false;
		}

		std::vector<ColumnMappingItem> mappings = prepareItems(mGUI->mMappingTableView->rowItems());
		if(mappings.size() == 0)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "No columns for output.");
			return false;
		}

		std::vector<DatabaseColumn *> srcColumns = oReader->getColumns();
		std::vector<DatabaseColumn *> tgtColumns;
		for(ColumnMappingItem &mi : mappings)
			tgtColumns.push_back(mi.getTargetColumn());

		int szm = mappings.size();
/*		for(int i = 0; i < srcColumns.size(); i++)
			srcColumns[i]->setPosition(i);
*/
		for(size_t i = 0; i < tgtColumns.size(); i++)
			tgtColumns[i]->setPosition(i);

		prg->setMaximum(prg->maximum()+4);
		int max_rows = oReader->count();
		StdString max_row_string;
		if(max_rows > 0)
		{
			prg->setMaximum(prg->maximum()+max_rows);
			max_row_string = "/"+spt::string::toString(max_rows);
		}
		prg->setValue(prg->value()+1);

		bool rc = oReader->begin();
		prg->setValue(prg->value()+1);
		if(rc == false)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "Unable to open source");
			return false;
		}

		rc = oWriter->prepareOpen(tgtColumns);
		prg->setValue(prg->value()+1);
		if(rc == false)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "unable to prepare the columns for the target.");
			oReader->end();
			return false;
		}

		rc = oWriter->begin();
		prg->setValue(prg->value()+1);
		if(rc == false)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "unable to open target");
			oReader->end();
			return false;
		}

		std::vector<StdString> inrow;
		std::vector<StdString> outrow;
		if(max_rows == -1)
		{
			// Enable animated bar. ???
			prg->setMinimum(0);
			prg->setMaximum(0);
			prg->setRange(0, 0);
		}

		while(1)
		{
			inrow.clear();
			outrow.clear();

			auto s = "Copying ... "+spt::string::toQt(spt::string::toString(n)+max_row_string);
			prg->setLabelText(s);
			QApplication::processEvents();
			if(prg->wasCanceled())
			{
				canceled = true;
				break;
			}

			int res = oReader->read(srcColumns, inrow);
			if(res <= 0)
			{
				if(res < 0)
				{
					ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "Error while reading from target at record "+spt::string::toString(n));
					ok = false;
				}
				break;
			}

			for(size_t i = 0; i < inrow.size(); i++)
				srcColumns.at(i)->setValue(inrow[i]);

			// Convert the string to the output format
			for(int i = 0; i < szm; i++)
			{
				ColumnMappingItem &mi = mappings[i];
				StdString *val = NULL;
				DatabaseColumn *src = mi.getSourceColumn();
				DatabaseColumn *tgt = mi.getTargetColumn();
				if(src && !src->isNull())
					val = new StdString(inrow[src->getPosition()]);

				val = mi.format(val);
				if(val)
				{
					tgt->setNull(false);
					outrow.push_back(*val);
					delete val;
				}
				else
				{
					tgt->setNull(true);
					outrow.push_back("");
				}
			}

			n++;
			res = oWriter->write(tgtColumns, outrow);
			if(res <= 0)
			{
				if(res < 0)
				{
					ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "Error while writing to target at record "+spt::string::toString(n));
					ok = false;
				}
				break;
			}

			if(max_rows > 0)
				prg->setValue(prg->value()+1);
		}

		oReader->end();
		if(ok && !canceled)
			oWriter->commit();
		else
			oWriter->rollback();

		oWriter->end();
	}
	catch(std::exception const &e)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", "Exception caught during copying.");
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "Copy", e.what());

		if(oReader)
			oReader->end();

		if(oWriter)
		{
			oWriter->rollback();
			oWriter->end();
		}

		return false;
	}

	prg->hide();

	if(!prg.hasMessages())
	{
		StdString s;
		if(ok)
			s = "Copying done!\n\n" + spt::string::toString(n)+" record(s) copied!";
		if(canceled)
			s = "Copying canceled!\n\nWARNING! Partial data may have been written!";

		QMessageBox msgBox;
		msgBox.setText("");
		msgBox.setInformativeText(spt::string::toQt(s));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
	}

	return true;
}

bool MainFrame::runProfile(StdString const *oProfileName)
{
	if(!loadProfile(oProfileName))
		return false;

	onCopy();

	return true;
}

bool MainFrame::loadProfile(StdString const *oProfileName)
{
	if(getTargetBox()->count() == 0 || getSourceBox()->count() == 0)
		return false;

	StdString fn;
	if(oProfileName == NULL)
	{
		auto fnl  = QFileDialog::getOpenFileName(this, tr("Open profile"), tr("."), tr("Datinator profile (*.datip);; All files (*.*)"));
		if(fnl.isNull())
			return false;

		fn = spt::string::fromQt(fnl);
	}
	else
		fn = *oProfileName;

	mRecentProfile->addItem(mRecentProfileTag, "", fn);

	Progress prg("Loading profile", "Loading profile...");

	StdString uuid;
	QScopedPointer<QSettings> prof(new QSettings(spt::string::toQt(fn), QSettings::IniFormat));
	prof->beginGroup("main");

	uuid = spt::string::fromQt(prof->value("targetContainer").toString());
	int i = findContainer(getTargetBox(), uuid);
	if(i == -1)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "LoadProfile", "The target container with the ID "+uuid+" doesn't exist");
		return false;
	}
	getTargetBox()->setCurrentIndex(i);
	onTargetSelected(i);

	uuid = spt::string::fromQt(prof->value("sourceContainer").toString());
	i = findContainer(getSourceBox(), uuid);
	if(i == -1)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "LoadProfile", "The source container with the ID "+uuid+" doesn't exist");
		return false;
	}
	getSourceBox()->setCurrentIndex(i);
	onSourceSelected(i);

	StdString t = spt::string::fromQt(prof->value("modeTruncate", "T").toString());
	if(t == "T")
		mGUI->mTruncateBtn->setChecked(true);
	else
		mGUI->mAppendBtn->setChecked(true);
	prof->endGroup();

	IDataContainerReader *sc = getCurrentSourceContainer();
	IDataContainerWriter *tc = getCurrentTargetContainer();

	/**** Target group ****/
	prof->beginGroup(spt::string::toQt(tc->getContainerUUID()));
		tc->loadProfile(*prof, "tgt");
	prof->endGroup();

	/**** Source group ****/
	prof->beginGroup(spt::string::toQt(sc->getContainerUUID()));
		sc->loadProfile(*prof, "src");
	prof->endGroup();

	prof->beginGroup("main");
		setRowcount(prof->value("rowCount").toInt());
	prof->endGroup();

	/**** Mapping group ****/
	prof->beginGroup("mapping");
		mGUI->mMappingTableView->setSourceColumns(sc->getColumns());
		mGUI->mMappingTableView->loadProfile(*prof);
	prof->endGroup();

	return true;
}

bool MainFrame::saveProfile(StdString const *oProfileName)
{
	if(getTargetBox()->count() == 0 || getSourceBox()->count() == 0)
		return false;

	StdString fn;
	if(oProfileName == NULL)
	{
		auto fnl = QFileDialog::getSaveFileName(this, tr("Save profile"), tr("."), tr("Datinator profile (*.datip);; All files (*.*)"));
		if(fnl.isNull())
			return false;

		fn = spt::string::fromQt(fnl);
	}
	else
		fn = *oProfileName;

	mRecentProfile->addItem(mRecentProfileTag, "", fn);

	QFile::remove(spt::string::toQt(fn));
	QScopedPointer<QSettings> prof(new QSettings(spt::string::toQt(fn), QSettings::IniFormat));

	IDataContainerReader *sc = getCurrentSourceContainer();
	IDataContainerWriter *tc = getCurrentTargetContainer();

	/**** main group ****/
	prof->beginGroup("main");

		prof->setValue("sourceContainer", spt::string::toQt(sc->getContainerUUID()));
		prof->setValue("targetContainer", spt::string::toQt(tc->getContainerUUID()));
		prof->setValue("rowCount", getRowcount());
		if(mGUI->mTruncateBtn->isChecked())
			prof->setValue("modeTruncate", "T");
		else
			prof->setValue("modeTruncate", "F");

	prof->endGroup();

	/**** Mapping group ****/
	prof->beginGroup("mapping");
		mGUI->mMappingTableView->saveProfile(*prof);
	prof->endGroup();

	/**** Source group ****/
	prof->beginGroup(spt::string::toQt(sc->getContainerUUID()));
		sc->saveProfile(*prof, "src");
	prof->endGroup();

	/**** Target group ****/
	prof->beginGroup(spt::string::toQt(tc->getContainerUUID()));
		tc->saveProfile(*prof, "tgt");
	prof->endGroup();

	prof->sync();

	return true;
}

/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QItemSelectionModel>
#include <QtWidgets/QMessageBox>

#include "manipulator/editor/manipulator_editor_dlg_gui.moc"
#include "manipulator/imanipulator.h"
#include "manipulator/null/null.h"
#include "manipulator/text/text.h"
#include "manipulator/date/date.h"
#include "manipulator/column/column.h"
#include "manipulator/autonumber/autonumber.h"

#include "support_qt/helper/gui_helper.h"

const char *gPropertyName = "Manipulator";

Q_DECLARE_METATYPE(IManipulator *)

QList<IManipulator *> initManipulators(void)
{
	QList<IManipulator *> l;

	l.append(new NullManipulator());
	l.append(new TextManipulator());
	l.append(new AutoNumberManipulator());
	l.append(new DateManipulator());
	l.append(new ColumnManipulator());

	return l;
}
QList<IManipulator *> gReferences = initManipulators();


ManipulatorEditorDialogBox::ManipulatorEditorDialogBox(QList<IManipulator *> const &oManipulators, QList<DatabaseColumn *> const &oColumns, QWidget *oParent)
: QDialog(oParent)
{
	mClear = true;
	mColumns = oColumns;

	mGUI = new Ui::ManipulatorEditorDlgGUI();
	mGUI->setupUi(this);

	QVBoxLayout *l = mGUI->mConfigLayout;
	mPlaceholder = new QLabel("", this);
	l->addWidget(mPlaceholder);

	supportlib::gui::setButtonIcon(supportlib::image::button_add, supportlib::image::button_add_length, mGUI->mAssignBtn, "Assign");
	supportlib::gui::setButtonIcon(supportlib::image::button_delete, supportlib::image::button_delete_length, mGUI->mRemoveBtn, "Remove");
	supportlib::gui::setButtonIcon(supportlib::image::button_down, supportlib::image::button_down_length, mGUI->mDownBtn, "Move down");
	supportlib::gui::setButtonIcon(supportlib::image::button_up, supportlib::image::button_up_length, mGUI->mUpBtn, "Move up");

	mGUI->mReferenceList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	mGUI->mAssignedList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	mGUI->mAssignBtn->setEnabled(false);
	mGUI->mRemoveBtn->setEnabled(false);
	mGUI->mUpBtn->setEnabled(false);
	mGUI->mDownBtn->setEnabled(false);

	QPalette p;
	QColor col = p.color(QPalette::Button);
	p.setColor(QPalette::Base, col);
	p.setColor(QPalette::Text, Qt::black);
	mGUI->mSampleTxt->setPalette(p);

	mManipulators = oManipulators;
	for(IManipulator * &mp : gReferences)
	{
		mp->setSourceColumns(oColumns);
		addItem(mGUI->mReferenceList, mp, true);
	}

	for(IManipulator * &mp : mManipulators)
	{
		mp->setSourceColumns(oColumns);
		addItem(mGUI->mAssignedList, mp->duplicate());
	}

	QString tst;
	IManipulator *mp = itemAt(mGUI->mAssignedList, 0);
	if(mp)
		tst = mp->getTestValue();
	else
		tst = "*VALUE*";

	mGUI->mTestValueTxt->setText(tst);
	updateSample();
}

ManipulatorEditorDialogBox::~ManipulatorEditorDialogBox(void)
{
}

QList<IManipulator *> ManipulatorEditorDialogBox::getManipulators(void)
{
	return mManipulators;
}

void ManipulatorEditorDialogBox::removeConfigWidget(void)
{
	QVBoxLayout *l = mGUI->mConfigLayout;
	QLayoutItem *li = l->itemAt(0);
	if(li)
	{
		QWidget *cw = li->widget();

		if(cw)
		{
			l->removeWidget(cw);
			cw->hide();
			QVariant v = cw->property(gPropertyName);
			IManipulator *cmp = v.value<IManipulator *>();
			if(cmp)
				cmp->removeConfigChangeListener(this);
		}
	}
}

void ManipulatorEditorDialogBox::accept(void)
{
	int rows = mGUI->mAssignedList->count();

	QListWidget *l = mGUI->mAssignedList;
	for(int i = 0; i < rows; i++)
	{
		IManipulator *mp = itemAt(l, i);
		if(!mp->isConfigured())
		{
			QMessageBox msgBox;
			msgBox.setText("Configuration missing!");
			QString s = "The manipulator ["+mp->getName()+"] at line "+QString::number(i)+" is not configured!\n";
			s += "Please configure all items before closing this dialog.\n\n";
			s += "Error message:"+mp->lastError();
			msgBox.setInformativeText(s);
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.exec();
			return;
		}
	}

	// When accepted, we can delete the original list
	// and put all currently assigned items into the
	// list.
	removeConfigWidget();
	mManipulators.clear();

	IManipulator *mp;
	while((mp = removeItem(0)) != NULL)
	{
		mp->removeConfigChangeListener(this);
		mManipulators.append(mp);
	}
	QDialog::accept();
}

void ManipulatorEditorDialogBox::reject(void)
{
	// When canceled, we can delete all manipulators
	// which are in the assigned list.
	removeConfigWidget();
	IManipulator *mp;
	while((mp = removeItem(0)) != NULL)
	{
		mp->removeConfigChangeListener(this);
		delete mp;
	}
	QDialog::reject();
}

void ManipulatorEditorDialogBox::onButton(QAbstractButton *oButton)
{
	if(mGUI->buttonBox->button(QDialogButtonBox::Reset) == oButton)
		onReset();
}

void ManipulatorEditorDialogBox::onReset(void)
{
}

void ManipulatorEditorDialogBox::onUp(void)
{
	QListWidget *l = mGUI->mAssignedList;
	QModelIndexList selected = l->selectionModel()->selectedRows();

	for(QModelIndex const &index : selected)
	{
		int r = index.row();
		if(r <= 0)
			return;

		QListWidgetItem *it0 = l->takeItem(r-1);
		l->insertItem(r, it0);
	}
	updateSample();
}

void ManipulatorEditorDialogBox::onDown(void)
{
	QListWidget *l = mGUI->mAssignedList;
	QModelIndexList selected = l->selectionModel()->selectedRows();
	int rows = l->count()-1;
	QList<QModelIndex> ml;
	for(QModelIndex const &index : selected)
		ml.insert(0, index);

	for(QModelIndex const &index : ml)
	{
		int r = index.row();
		if(r >= rows)
			return;

		QListWidgetItem *it0 = l->takeItem(r+1);
		l->insertItem(r, it0);
	}
	updateSample();
}

void ManipulatorEditorDialogBox::onAssign(void)
{
	QModelIndexList selected = mGUI->mReferenceList->selectionModel()->selectedRows();
	QListWidget *l = mGUI->mAssignedList;
	for(QModelIndex const &index : selected)
	{
		IManipulator *mp = itemAt(mGUI->mReferenceList, index.row());
		mp = mp->duplicate();
		mp->getConfigurationPanel(this);
		addItem(l, mp);
	}

	updateSample();
}

void ManipulatorEditorDialogBox::onRemove(void)
{
	removeConfigWidget();
	QListWidget *l = mGUI->mAssignedList;
	QModelIndexList selected = l->selectionModel()->selectedRows();
	for(QModelIndex const &index : selected)
	{
		IManipulator *mp = itemAt(l, index.row());
		mp->removeConfigChangeListener(this);
		removeItem(index.row());
		delete mp;
	}

	updateSample();
}

void ManipulatorEditorDialogBox::onReferenceSelectionChanged(void)
{
	if(mClear)
	{
		mClear = false;
		mGUI->mAssignedList->selectionModel()->clearSelection();
		mClear = true;
	}

	QModelIndexList selected = mGUI->mReferenceList->selectionModel()->selectedRows();
	if(selected.size() == 0)
	{
		mGUI->mAssignBtn->setEnabled(false);
		mGUI->mRemoveBtn->setEnabled(false);
		mGUI->mUpBtn->setEnabled(false);
		mGUI->mDownBtn->setEnabled(false);
	}
	else
	{
		mGUI->mAssignBtn->setEnabled(true);
		mGUI->mRemoveBtn->setEnabled(false);
		mGUI->mUpBtn->setEnabled(false);
		mGUI->mDownBtn->setEnabled(false);
	}
}

void ManipulatorEditorDialogBox::onAssignedSelectionChanged(void)
{
	if(mClear)
	{
		mClear = false;
		mGUI->mReferenceList->selectionModel()->clearSelection();
		mClear = true;
	}

	QModelIndexList selected = mGUI->mAssignedList->selectionModel()->selectedRows();
	if(selected.size() == 0)
	{
		mGUI->mAssignBtn->setEnabled(false);
		mGUI->mRemoveBtn->setEnabled(false);
		mGUI->mUpBtn->setEnabled(false);
		mGUI->mDownBtn->setEnabled(false);
	}
	else
	{
		mGUI->mAssignBtn->setEnabled(false);
		mGUI->mRemoveBtn->setEnabled(true);
		mGUI->mUpBtn->setEnabled(true);
		mGUI->mDownBtn->setEnabled(true);
	}

	QVBoxLayout *l = mGUI->mConfigLayout;
	removeConfigWidget();
	QWidget *w = NULL;
	if(selected.size() == 1)
	{
		IManipulator *mp = itemAt(mGUI->mAssignedList, selected.at(0).row());
		if(mp)
		{
			w = mp->getConfigurationPanel(this);

			if(w)
			{
				l->addWidget(w);
				w->show();
				QVariant v;
				v.setValue(mp);
				mp->addConfigChangeListener(this);
				w->setProperty(gPropertyName, v);
			}
		}
	}

	if(!w)
	{
		l->addWidget(mPlaceholder);
		mPlaceholder->show();
	}
}

IManipulator *ManipulatorEditorDialogBox::itemAt(QListWidget *oListView, int nRow)
{
	if(nRow >= oListView->count())
		return NULL;

	QListWidgetItem *it = oListView->item(nRow);
	QVariant v(it->data(Qt::UserRole));
	return v.value<IManipulator *>();
}

void ManipulatorEditorDialogBox::addItem(QListWidget *oListView, IManipulator *oManipulator, bool bReference)
{
	QListWidgetItem *it = new QListWidgetItem();
	QString s;
	if(bReference)
		s = oManipulator->getName();
	else
		s = oManipulator->toString();

	QVariant v;
	it->setText(s);
	it->setToolTip(oManipulator->getDescription());
	v.setValue(oManipulator);
	it->setData(Qt::UserRole, v);
	oListView->addItem(it);
}

IManipulator *ManipulatorEditorDialogBox::removeItem(int nRow)
{
	removeConfigWidget();
	QListWidget *l = mGUI->mAssignedList;
	QListWidgetItem *it = l->takeItem(nRow);
	if(!it)
		return NULL;

	QVariant v = it->data(Qt::UserRole);
	IManipulator *mp = v.value<IManipulator *>();
	delete it;

	return mp;
}

void ManipulatorEditorDialogBox::updateItem(int nRow, IManipulator *oManipulator)
{
	QListWidget *l = mGUI->mAssignedList;
	QListWidgetItem *it = l->item(nRow);
	if(!oManipulator)
	{
		QVariant v = it->data(Qt::UserRole);
		oManipulator = v.value<IManipulator *>();
	}

	it->setText(oManipulator->toString());
	l->itemChanged(it);
}

void ManipulatorEditorDialogBox::handleNotification(Dispatcher<IManipulator *> *oDispatcher, IManipulator *oSource)
{
	UNUSED(oDispatcher);

	updateSample(oSource);
}

void ManipulatorEditorDialogBox::onTestValueChanged(QString oValue)
{
	UNUSED(oValue);

	IManipulator *mp = NULL;
	QModelIndexList selected = mGUI->mAssignedList->selectionModel()->selectedRows();
	if(selected.size() == 1)
		mp = itemAt(mGUI->mAssignedList, selected.at(0).row());
	updateSample(mp);
}

void ManipulatorEditorDialogBox::updateSample(IManipulator *oSource)
{
	QListWidget *l = mGUI->mAssignedList;
	int rows = l->count();
	QString t;
	QString tst = mGUI->mTestValueTxt->text();
	QString *s = new QString(tst);
	QString *del = NULL;

	for(int i = 0; i < rows; i++)
	{
		IManipulator *mp = itemAt(l, i);
		if(i == 0)
			mp->setTestValue(tst);
		else
			mp->setTestValue("");
		mp->prepare();
		if(mp == oSource)
			updateItem(i, mp);

		s = mp->format(s, true);
	}

	del = s;

	// Reset to the original state, so the configuration doesn't get messed up.
	for(int i = 0; i < rows; i++)
	{
		IManipulator *mp = itemAt(l, i);
		mp->prepare();
	}

	if(!s)
	{
		t = "[null]";
		s = &t;
	}

	mGUI->mSampleTxt->setText(*s);
	if(del)
		delete del;
}

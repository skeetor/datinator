/********************************************************************************
** Form generated from reading UI file 'logging_dialog_box_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LOGGING_DIALOG_BOX_GUI_H
#define LOGGING_DIALOG_BOX_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_LoggingDialogGUI
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *mButtonBox;
    QTableView *mItemTableView;

    void setupUi(QDialog *LoggingDialogGUI)
    {
        if (LoggingDialogGUI->objectName().isEmpty())
            LoggingDialogGUI->setObjectName(QStringLiteral("LoggingDialogGUI"));
        LoggingDialogGUI->resize(494, 251);
        gridLayout = new QGridLayout(LoggingDialogGUI);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        mButtonBox = new QDialogButtonBox(LoggingDialogGUI);
        mButtonBox->setObjectName(QStringLiteral("mButtonBox"));
        mButtonBox->setOrientation(Qt::Horizontal);
        mButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(mButtonBox, 1, 0, 1, 1);

        mItemTableView = new QTableView(LoggingDialogGUI);
        mItemTableView->setObjectName(QStringLiteral("mItemTableView"));

        gridLayout->addWidget(mItemTableView, 0, 0, 1, 1);


        retranslateUi(LoggingDialogGUI);
        QObject::connect(mButtonBox, SIGNAL(accepted()), LoggingDialogGUI, SLOT(accept()));
        QObject::connect(mButtonBox, SIGNAL(rejected()), LoggingDialogGUI, SLOT(reject()));

        QMetaObject::connectSlotsByName(LoggingDialogGUI);
    } // setupUi

    void retranslateUi(QDialog *LoggingDialogGUI)
    {
        LoggingDialogGUI->setWindowTitle(QApplication::translate("LoggingDialogGUI", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class LoggingDialogGUI: public Ui_LoggingDialogGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LOGGING_DIALOG_BOX_GUI_H

/********************************************************************************
** Form generated from reading UI file 'database_login_panel_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DATABASE_LOGIN_PANEL_GUI_H
#define DATABASE_LOGIN_PANEL_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DatabaseLoginGUI
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QCheckBox *mSaveHostBox;
    QLabel *label_4;
    QLineEdit *mHostTxt;
    QVBoxLayout *verticalLayout_2;
    QPushButton *mSaveBtn;
    QPushButton *mRemoveBtn;
    QPushButton *mMoveUpBtn;
    QPushButton *mMoveDownBtn;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QLineEdit *mUserTxt;
    QLabel *label_3;
    QLineEdit *mPasswordTxt;
    QLabel *label;
    QLineEdit *mDatabaseTxt;
    QListView *mLoginListView;

    void setupUi(QFrame *DatabaseLoginGUI)
    {
        if (DatabaseLoginGUI->objectName().isEmpty())
            DatabaseLoginGUI->setObjectName(QStringLiteral("DatabaseLoginGUI"));
        DatabaseLoginGUI->resize(435, 186);
        DatabaseLoginGUI->setFrameShape(QFrame::Box);
        DatabaseLoginGUI->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(DatabaseLoginGUI);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        mSaveHostBox = new QCheckBox(DatabaseLoginGUI);
        mSaveHostBox->setObjectName(QStringLiteral("mSaveHostBox"));

        gridLayout->addWidget(mSaveHostBox, 1, 1, 1, 1);

        label_4 = new QLabel(DatabaseLoginGUI);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1, Qt::AlignTop);

        mHostTxt = new QLineEdit(DatabaseLoginGUI);
        mHostTxt->setObjectName(QStringLiteral("mHostTxt"));

        gridLayout->addWidget(mHostTxt, 2, 0, 1, 3, Qt::AlignTop);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        mSaveBtn = new QPushButton(DatabaseLoginGUI);
        mSaveBtn->setObjectName(QStringLiteral("mSaveBtn"));

        verticalLayout_2->addWidget(mSaveBtn, 0, Qt::AlignTop);

        mRemoveBtn = new QPushButton(DatabaseLoginGUI);
        mRemoveBtn->setObjectName(QStringLiteral("mRemoveBtn"));

        verticalLayout_2->addWidget(mRemoveBtn, 0, Qt::AlignTop);

        mMoveUpBtn = new QPushButton(DatabaseLoginGUI);
        mMoveUpBtn->setObjectName(QStringLiteral("mMoveUpBtn"));

        verticalLayout_2->addWidget(mMoveUpBtn, 0, Qt::AlignBottom);

        mMoveDownBtn = new QPushButton(DatabaseLoginGUI);
        mMoveDownBtn->setObjectName(QStringLiteral("mMoveDownBtn"));

        verticalLayout_2->addWidget(mMoveDownBtn, 0, Qt::AlignBottom);


        gridLayout->addLayout(verticalLayout_2, 0, 2, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        label_2 = new QLabel(DatabaseLoginGUI);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_3->addWidget(label_2, 0, Qt::AlignTop);

        mUserTxt = new QLineEdit(DatabaseLoginGUI);
        mUserTxt->setObjectName(QStringLiteral("mUserTxt"));

        verticalLayout_3->addWidget(mUserTxt);

        label_3 = new QLabel(DatabaseLoginGUI);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3, 0, Qt::AlignTop);

        mPasswordTxt = new QLineEdit(DatabaseLoginGUI);
        mPasswordTxt->setObjectName(QStringLiteral("mPasswordTxt"));
        mPasswordTxt->setEchoMode(QLineEdit::PasswordEchoOnEdit);

        verticalLayout_3->addWidget(mPasswordTxt, 0, Qt::AlignTop);

        label = new QLabel(DatabaseLoginGUI);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label, 0, Qt::AlignTop);

        mDatabaseTxt = new QLineEdit(DatabaseLoginGUI);
        mDatabaseTxt->setObjectName(QStringLiteral("mDatabaseTxt"));

        verticalLayout_3->addWidget(mDatabaseTxt, 0, Qt::AlignTop);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 2);

        mLoginListView = new QListView(DatabaseLoginGUI);
        mLoginListView->setObjectName(QStringLiteral("mLoginListView"));

        gridLayout->addWidget(mLoginListView, 0, 3, 4, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(3, 2);

        verticalLayout->addLayout(gridLayout);


        retranslateUi(DatabaseLoginGUI);
        QObject::connect(mSaveBtn, SIGNAL(clicked()), DatabaseLoginGUI, SLOT(onSave()));
        QObject::connect(mRemoveBtn, SIGNAL(clicked()), DatabaseLoginGUI, SLOT(onRemove()));
        QObject::connect(mMoveUpBtn, SIGNAL(clicked()), DatabaseLoginGUI, SLOT(onMoveUp()));
        QObject::connect(mMoveDownBtn, SIGNAL(clicked()), DatabaseLoginGUI, SLOT(onMoveDown()));
        QObject::connect(mLoginListView, SIGNAL(clicked(QModelIndex)), DatabaseLoginGUI, SLOT(onItemClicked(QModelIndex)));

        QMetaObject::connectSlotsByName(DatabaseLoginGUI);
    } // setupUi

    void retranslateUi(QFrame *DatabaseLoginGUI)
    {
        DatabaseLoginGUI->setWindowTitle(QString());
        mSaveHostBox->setText(QApplication::translate("DatabaseLoginGUI", "Save host", 0));
        label_4->setText(QApplication::translate("DatabaseLoginGUI", "Host", 0));
        mSaveBtn->setText(QString());
        mRemoveBtn->setText(QString());
        mMoveUpBtn->setText(QString());
        mMoveDownBtn->setText(QString());
        label_2->setText(QApplication::translate("DatabaseLoginGUI", "Username", 0));
        label_3->setText(QApplication::translate("DatabaseLoginGUI", "Password", 0));
        label->setText(QApplication::translate("DatabaseLoginGUI", "Database", 0));
    } // retranslateUi

};

namespace Ui {
    class DatabaseLoginGUI: public Ui_DatabaseLoginGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DATABASE_LOGIN_PANEL_GUI_H

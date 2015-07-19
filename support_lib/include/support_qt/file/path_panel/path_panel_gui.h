/********************************************************************************
** Form generated from reading UI file 'path_panel_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PATH_PANEL_GUI_H
#define PATH_PANEL_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PathSelectGUI
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *mPathLabel;
    QLineEdit *mPathText;
    QPushButton *mBrowseBtn;
    QComboBox *mEncodingBox;

    void setupUi(QFrame *PathSelectGUI)
    {
        if (PathSelectGUI->objectName().isEmpty())
            PathSelectGUI->setObjectName(QStringLiteral("PathSelectGUI"));
        PathSelectGUI->resize(403, 220);
        PathSelectGUI->setFrameShape(QFrame::Box);
        PathSelectGUI->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(PathSelectGUI);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mPathLabel = new QLabel(PathSelectGUI);
        mPathLabel->setObjectName(QStringLiteral("mPathLabel"));

        horizontalLayout->addWidget(mPathLabel, 0, Qt::AlignTop);

        mPathText = new QLineEdit(PathSelectGUI);
        mPathText->setObjectName(QStringLiteral("mPathText"));

        horizontalLayout->addWidget(mPathText, 0, Qt::AlignTop);

        mBrowseBtn = new QPushButton(PathSelectGUI);
        mBrowseBtn->setObjectName(QStringLiteral("mBrowseBtn"));

        horizontalLayout->addWidget(mBrowseBtn, 0, Qt::AlignTop);

        mEncodingBox = new QComboBox(PathSelectGUI);
        mEncodingBox->setObjectName(QStringLiteral("mEncodingBox"));
        mEncodingBox->setEnabled(false);

        horizontalLayout->addWidget(mEncodingBox, 0, Qt::AlignTop);

        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(PathSelectGUI);
        QObject::connect(mBrowseBtn, SIGNAL(clicked()), PathSelectGUI, SLOT(onBrowse()));

        QMetaObject::connectSlotsByName(PathSelectGUI);
    } // setupUi

    void retranslateUi(QFrame *PathSelectGUI)
    {
        PathSelectGUI->setWindowTitle(QString());
        mPathLabel->setText(QApplication::translate("PathSelectGUI", "Path", 0));
        mBrowseBtn->setText(QApplication::translate("PathSelectGUI", "Browse ...", 0));
    } // retranslateUi

};

namespace Ui {
    class PathSelectGUI: public Ui_PathSelectGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PATH_PANEL_GUI_H

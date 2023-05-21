/********************************************************************************
** Form generated from reading UI file 'ControlBar.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLBAR_H
#define UI_CONTROLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlBar
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEditTime;
    QRadioButton *rbP1Human;
    QLabel *lblTime;
    QLineEdit *lineEditSize;
    QLabel *lblSize;
    QRadioButton *rbP1AI;
    QRadioButton *rbP2AI;
    QRadioButton *rbP2Human;
    QPushButton *pbTraining;
    QPushButton *pbGame;

    void setupUi(QWidget *ControlBar)
    {
        if (ControlBar->objectName().isEmpty())
            ControlBar->setObjectName("ControlBar");
        ControlBar->resize(400, 300);
        gridLayout = new QGridLayout(ControlBar);
        gridLayout->setObjectName("gridLayout");
        lineEditTime = new QLineEdit(ControlBar);
        lineEditTime->setObjectName("lineEditTime");

        gridLayout->addWidget(lineEditTime, 1, 1, 1, 1);

        rbP1Human = new QRadioButton(ControlBar);
        rbP1Human->setObjectName("rbP1Human");
        QFont font;
        font.setBold(true);
        rbP1Human->setFont(font);
        rbP1Human->setAutoExclusive(false);

        gridLayout->addWidget(rbP1Human, 1, 2, 1, 1);

        lblTime = new QLabel(ControlBar);
        lblTime->setObjectName("lblTime");
        lblTime->setFont(font);
        lblTime->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lblTime, 0, 1, 1, 1);

        lineEditSize = new QLineEdit(ControlBar);
        lineEditSize->setObjectName("lineEditSize");

        gridLayout->addWidget(lineEditSize, 1, 0, 1, 1);

        lblSize = new QLabel(ControlBar);
        lblSize->setObjectName("lblSize");
        lblSize->setFont(font);
        lblSize->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lblSize, 0, 0, 1, 1);

        rbP1AI = new QRadioButton(ControlBar);
        rbP1AI->setObjectName("rbP1AI");
        rbP1AI->setFont(font);
        rbP1AI->setChecked(true);
        rbP1AI->setAutoExclusive(false);

        gridLayout->addWidget(rbP1AI, 0, 2, 1, 1);

        rbP2AI = new QRadioButton(ControlBar);
        rbP2AI->setObjectName("rbP2AI");
        rbP2AI->setFont(font);
        rbP2AI->setChecked(true);
        rbP2AI->setAutoExclusive(false);

        gridLayout->addWidget(rbP2AI, 0, 3, 1, 1);

        rbP2Human = new QRadioButton(ControlBar);
        rbP2Human->setObjectName("rbP2Human");
        rbP2Human->setFont(font);
        rbP2Human->setAutoExclusive(false);

        gridLayout->addWidget(rbP2Human, 1, 3, 1, 1);

        pbTraining = new QPushButton(ControlBar);
        pbTraining->setObjectName("pbTraining");

        gridLayout->addWidget(pbTraining, 0, 4, 1, 1);

        pbGame = new QPushButton(ControlBar);
        pbGame->setObjectName("pbGame");

        gridLayout->addWidget(pbGame, 1, 4, 1, 1);

        gridLayout->setRowStretch(0, 1);
        gridLayout->setRowStretch(1, 1);
        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 1);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 1);
        gridLayout->setColumnStretch(4, 1);

        retranslateUi(ControlBar);

        QMetaObject::connectSlotsByName(ControlBar);
    } // setupUi

    void retranslateUi(QWidget *ControlBar)
    {
        ControlBar->setWindowTitle(QCoreApplication::translate("ControlBar", "Form", nullptr));
        rbP1Human->setText(QCoreApplication::translate("ControlBar", "P1 Human", nullptr));
        lblTime->setText(QCoreApplication::translate("ControlBar", "Time", nullptr));
        lblSize->setText(QCoreApplication::translate("ControlBar", "Size", nullptr));
        rbP1AI->setText(QCoreApplication::translate("ControlBar", "P1 AI", nullptr));
        rbP2AI->setText(QCoreApplication::translate("ControlBar", "P2 AI", nullptr));
        rbP2Human->setText(QCoreApplication::translate("ControlBar", "P2 Human", nullptr));
        pbTraining->setText(QCoreApplication::translate("ControlBar", "Training", nullptr));
        pbGame->setText(QCoreApplication::translate("ControlBar", "Game", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControlBar: public Ui_ControlBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLBAR_H

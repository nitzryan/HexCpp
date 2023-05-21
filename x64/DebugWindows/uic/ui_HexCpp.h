/********************************************************************************
** Form generated from reading UI file 'HexCpp.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEXCPP_H
#define UI_HEXCPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "UI/ControlBar.h"
#include "UI/EvalBar.h"
#include "UI/EvalTree.h"
#include "UI/HexGrid.h"

QT_BEGIN_NAMESPACE

class Ui_HexCppClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    EvalBar *evalBar;
    QHBoxLayout *horizontalLayout;
    EvalTree *evalTree;
    HexGrid *hexGrid;
    QWidget *trainingWidget;
    ControlBar *controlBar;

    void setupUi(QMainWindow *HexCppClass)
    {
        if (HexCppClass->objectName().isEmpty())
            HexCppClass->setObjectName("HexCppClass");
        HexCppClass->resize(600, 400);
        centralWidget = new QWidget(HexCppClass);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        evalBar = new EvalBar(centralWidget);
        evalBar->setObjectName("evalBar");

        verticalLayout->addWidget(evalBar);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        evalTree = new EvalTree(centralWidget);
        evalTree->setObjectName("evalTree");

        horizontalLayout->addWidget(evalTree);

        hexGrid = new HexGrid(centralWidget);
        hexGrid->setObjectName("hexGrid");

        horizontalLayout->addWidget(hexGrid);

        trainingWidget = new QWidget(centralWidget);
        trainingWidget->setObjectName("trainingWidget");

        horizontalLayout->addWidget(trainingWidget);

        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);

        controlBar = new ControlBar(centralWidget);
        controlBar->setObjectName("controlBar");

        verticalLayout->addWidget(controlBar);

        verticalLayout->setStretch(1, 1);
        HexCppClass->setCentralWidget(centralWidget);

        retranslateUi(HexCppClass);

        QMetaObject::connectSlotsByName(HexCppClass);
    } // setupUi

    void retranslateUi(QMainWindow *HexCppClass)
    {
        HexCppClass->setWindowTitle(QCoreApplication::translate("HexCppClass", "HexCpp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HexCppClass: public Ui_HexCppClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEXCPP_H

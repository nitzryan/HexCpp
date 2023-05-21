/********************************************************************************
** Form generated from reading UI file 'HexGrid.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEXGRID_H
#define UI_HEXGRID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HexGrid
{
public:

    void setupUi(QWidget *HexGrid)
    {
        if (HexGrid->objectName().isEmpty())
            HexGrid->setObjectName("HexGrid");
        HexGrid->resize(400, 300);

        retranslateUi(HexGrid);

        QMetaObject::connectSlotsByName(HexGrid);
    } // setupUi

    void retranslateUi(QWidget *HexGrid)
    {
        HexGrid->setWindowTitle(QCoreApplication::translate("HexGrid", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HexGrid: public Ui_HexGrid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEXGRID_H

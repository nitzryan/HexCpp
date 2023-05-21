#ifndef HEXCPP_H
#define HEXCPP_H

#include <QtWidgets/QMainWindow>
#include "ui_HexCpp.h"
#include "UI/HexController.h"

class HexCpp : public QMainWindow
{
    Q_OBJECT

public:
    HexCpp(QWidget *parent = nullptr);
    ~HexCpp();

private:
    Ui::HexCppClass ui;
    HexController* controller;
};

#endif
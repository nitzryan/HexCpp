#include "ControlBar.h"
#include "ui_ControlBar.h"

#include <QButtonGroup>
#include <QIntValidator>
#include <QDoubleValidator>

ControlBar::ControlBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlBar)
{
    ui->setupUi(this);

    // Initial state
    size = 0;
    time = 0;
    p1AI = true;
    p2AI = true;

    // Radio Buttons
    QButtonGroup* p1Group = new QButtonGroup(this);
    p1Group->addButton(ui->rbP1AI);
    p1Group->addButton(ui->rbP1Human);
    QButtonGroup* p2Group = new QButtonGroup(this);
    p2Group->addButton(ui->rbP2AI);
    p2Group->addButton(ui->rbP2Human);

    // LineEdit
    ui->lineEditSize->setValidator(new QIntValidator(1,25));
    QObject::connect(ui->lineEditSize, &QLineEdit::textChanged, [=](const QString &text) {
        int pos = 0;
        QString temp(text);
        if (ui->lineEditSize->validator()->validate(temp, pos) == QValidator::Acceptable) {
            ui->lineEditSize->setStyleSheet("background-color: white;");
            size = temp.toInt();
        } else {
            ui->lineEditSize->setStyleSheet("background-color: red;");
            size = 0;
        }
    });
    ui->lineEditSize->setText("7");

    ui->lineEditTime->setValidator(new QDoubleValidator(0.0001, 30, 4));
    QObject::connect(ui->lineEditTime, &QLineEdit::textChanged, [=](const QString &text) {
        int pos = 0;
        QString temp(text);
        if (ui->lineEditTime->validator()->validate(temp, pos) == QValidator::Acceptable) {
            ui->lineEditTime->setStyleSheet("background-color: white;");
            time = temp.toFloat();
        } else {
            ui->lineEditTime->setStyleSheet("background-color: red;");
            time = 0;
        }
    });
    ui->lineEditTime->setText("1");

    // Pushbuttons
    QObject::connect(ui->pbGame, &QPushButton::clicked, [=]() {
        if (time > 0 && size > 0)
            emit gameRequested(size, time, p1AI, p2AI);
    });
    QObject::connect(ui->pbTraining, &QPushButton::clicked, [=]() {
        if (time > 0 && size > 0)
            emit trainingRequested(size, time);
    });

    // Radio Buttons
    QObject::connect(ui->rbP1AI, &QRadioButton::clicked, [=]() {
        p1AI = true;
    });
    QObject::connect(ui->rbP1Human, &QRadioButton::clicked, [=]() {
        p1AI = false;
    });
    QObject::connect(ui->rbP2AI, &QRadioButton::clicked, [=]() {
        p2AI = true;
    });
    QObject::connect(ui->rbP2Human, &QRadioButton::clicked, [=]() {
        p2AI = false;
    });
}

ControlBar::~ControlBar()
{
    delete ui;
}


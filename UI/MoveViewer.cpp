#include "MoveViewer.h"
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QLabel>

MoveViewer::MoveViewer(QWidget* parent) : QWidget(parent)
{
	currentPosition = -1;

	layout = new QGridLayout(this);

	// Text
	QLabel* tb = new QLabel("Eval Time");
	layout->addWidget(tb, 0, 0);

	// Blank board
	QPushButton* startButton = new QPushButton("Start");
	QObject::connect(startButton, &QPushButton::clicked, [&]() {
		UpdatePosition(0);
		});
	layout->addWidget(startButton, 0, 1);

	// Add button to eval current position
	QLineEdit* lineEdit = new QLineEdit();
	lineEdit->setText(QString::number(currentTime));
	QDoubleValidator* validator = new QDoubleValidator(0.0, std::numeric_limits<double>::max(), 2, lineEdit);
	validator->setNotation(QDoubleValidator::StandardNotation);
	validator->setDecimals(2);
	lineEdit->setValidator(validator);
	QObject::connect(lineEdit, &QLineEdit::editingFinished, [&, lineEdit]() {
		QString text = lineEdit->text();
		bool ok;
		float value = text.toFloat(&ok);
		if (ok) {
			currentTime = value;
		}
		});
	layout->addWidget(lineEdit, 1, 0);
	
	// Add button to start eval
	QPushButton* EvalButton = new QPushButton("Eval");
	QObject::connect(EvalButton, &QPushButton::clicked, [&]() {
		emit EvaluatePosition(currentTime);
		});
	layout->addWidget(EvalButton, 1, 1);
}

void MoveViewer::SetMoveList(std::vector<std::string> list)
{
	moves = list;
	currentPosition = (int)moves.size() - 1;

	//Remove all buttons
	QLayoutItem* item;
	int index = 4;
	while ((item = layout->takeAt(index)) != nullptr) {
		delete item->widget();
		delete item;
	}

	

	for (size_t i = 0; i < moves.size(); i++) {
		QPushButton* button = new QPushButton(QString(moves.at(i).c_str()));
		QObject::connect(button, &QPushButton::clicked, [&, i]() {
			UpdatePosition((int)i + 1);
		});
		layout->addWidget(button, (int)(i / 2) + 2, (int)i % 2);
	}
}

void MoveViewer::UpdatePosition(int newPos) {
	currentPosition = newPos;
	
	// Validate input, clamp to valid range
	if (currentPosition > moves.size()) {
		currentPosition = (int)moves.size();
	}
	else if (currentPosition < 0) {
		currentPosition = 0;
	}

	emit MoveSelected(currentPosition);
}
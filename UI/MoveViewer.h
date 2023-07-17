#ifndef MOVEVIEWER_H
#define MOVEIVEWER_H

#include <QWidget>
#include <QGridLayout>
#include <string>
#include <vector>

class MoveViewer : public QWidget {
	Q_OBJECT
public:
	MoveViewer(QWidget* parent = nullptr);

public slots:
	void SetMoveList(std::vector<std::string> list);

signals:
	void MoveSelected(int move);
	void EvaluatePosition(float time);
	void CommitPosition();

private:
	std::vector<std::string> moves;
	int currentPosition;
	float currentTime = 1.0f;

	QGridLayout* layout;

	void UpdatePosition(int newPos);
};

#endif

#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>

namespace Ui {
class ControlBar;
}

class ControlBar : public QWidget
{
    Q_OBJECT

public:
    explicit ControlBar(QWidget *parent = nullptr);
    ~ControlBar();

signals:
    void gameRequested(int size, float time, bool P1AI, bool P2AI);
    void trainingRequested(int size, float time);

private:
    Ui::ControlBar *ui;
    int size;
    float time;
    bool p1AI;
    bool p2AI;
};

#endif // CONTROLBAR_H

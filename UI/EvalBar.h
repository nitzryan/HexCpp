#ifndef EVALBAR_H
#define EVALBAR_H

#include <QWidget>
#include <string>

class EvalBar : public QWidget
{
    Q_OBJECT
public:
    EvalBar(QWidget *parent = nullptr);
    void setScore(float score);
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    float score;
};

#endif // EVALBAR_H

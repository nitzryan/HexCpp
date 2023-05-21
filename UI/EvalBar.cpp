#include "EvalBar.h"

#include <QPainter>

#define MIN_HEIGHT 80
#define TEXT_PADDING 10

EvalBar::EvalBar(QWidget *parent)
    : QWidget{parent}
{
    setMinimumHeight(MIN_HEIGHT);
    setScore(0);
}

void EvalBar::setScore(float s)
{
    this->score = s;
    this->update();
}

float scoreToPercentage(float s)
{
    float absS = abs(s);
    float logScore = log10(absS + 1) / log10(1000);
    if (logScore > 1)
    {
        logScore = 1;
    }

    logScore *= 0.5;

    if (s > 0)
    {
        return 0.5 - logScore;
    }
    return 0.5 + logScore;
}

void EvalBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QRectF rect = this->rect();

    // Fill blue, then red
    auto percentage = scoreToPercentage(score);
    auto dividingPoint = rect.width() * percentage;
    painter.fillRect(rect.left(), rect.top(), dividingPoint, rect.height(), Qt::blue);
    painter.fillRect(dividingPoint, rect.top(), rect.right(), rect.height(), Qt::red);

    // Write text
    QRectF textRect;
    if (percentage > 0.5)
    {
        textRect = painter.boundingRect(rect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(score));
        textRect.setLeft(textRect.left() + TEXT_PADDING);
        textRect.setRight(textRect.right() + TEXT_PADDING);
    } else
    {
        textRect = painter.boundingRect(rect, Qt::AlignRight | Qt::AlignVCenter, QString::number(score));
        textRect.setLeft(textRect.left() - TEXT_PADDING);
        textRect.setRight(textRect.right() - TEXT_PADDING);
    }

    QFont font = painter.font();
    font.setWeight(QFont::Bold);
    painter.setFont(font);
    painter.drawText(textRect, Qt::AlignCenter, QString::number(score));
}

#ifndef HEXGRID_H
#define HEXGRID_H

#include <QWidget>
#include <vector>

#include "UI.h"

class HexGrid : public QWidget
{
    Q_OBJECT

public:
    explicit HexGrid(QWidget *parent = nullptr);
    ~HexGrid();

signals:
    void tileClicked(int tile);

public slots:
    void BoardState(std::vector<UI::Tile> tiles);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void drawHexagon(QPainter& painter, int col, int row);
    void drawBorder(QPainter& painter);
    int getTileClicked(qreal xMousePos, qreal yMousePos);
    void getTilePosition(qreal& xPos, qreal& yPos, int col, int row);

    int size;
    std::vector<UI::Tile> tiles;
    qreal scaleFactor;
};

#endif // HEXGRID_H

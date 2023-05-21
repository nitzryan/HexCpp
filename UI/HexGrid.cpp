#include "HexGrid.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#define TILE_SIZE 100.0
#define CLICK_MAX_DISTANCE TILE_SIZE
#define ROW_GAP 0.740 * TILE_SIZE
#define COL_GAP 0.866 * TILE_SIZE

HexGrid::HexGrid(QWidget *parent) :
    QWidget(parent)
{
    scaleFactor = 0;
    size = 0;
}

void HexGrid::BoardState(std::vector<UI::Tile> ts)
{
    this->size = sqrt((int)ts.size());
    this->tiles = ts;
    this->update();
}

HexGrid::~HexGrid()
{

}

void HexGrid::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setWindow(0,0,width(), height());
    painter.setViewport(0,0,width(), height());
    scaleFactor = qMin(width() / (TILE_SIZE * (size * 1.5 + 1)),
                             height() / (TILE_SIZE + (size * ROW_GAP)));
    painter.scale(scaleFactor, scaleFactor);

    for (auto i = 0; i < size; i++)
        for (auto j = 0; j < size; j++)
            drawHexagon(painter, i, j);

    drawBorder(painter);
}

void HexGrid::mousePressEvent(QMouseEvent *event)
{
    QPointF mousePoint = event->pos();
    mousePoint.setX(mousePoint.x() / scaleFactor);
    mousePoint.setY(mousePoint.y() / scaleFactor);
    int tile = getTileClicked(mousePoint.x(), mousePoint.y());
    if (tile != -1)
    {
        emit tileClicked(tile);
    }
}

void HexGrid::drawHexagon(QPainter &painter, int col, int row)
{
    static const QPolygonF hexagon = {
            QPointF(-0.433 * TILE_SIZE, 0.25 * TILE_SIZE),
            QPointF(0, TILE_SIZE / 2),
            QPointF(0.433 * TILE_SIZE, 0.25 * TILE_SIZE),
            QPointF(0.433 * TILE_SIZE, -0.25 * TILE_SIZE),
            QPointF(0, -TILE_SIZE / 2),
            QPointF(-0.433 * TILE_SIZE, -0.25 * TILE_SIZE),
            QPointF(-0.433 * TILE_SIZE, 0.25 * TILE_SIZE)
        };

    // Get tile centerpoint
    qreal x;
    qreal y;
    getTilePosition(x, y, col, row);

    // Draw tile
    painter.save();
    painter.translate(x,y);
    QPainterPath path;
    path.addPolygon(hexagon);

    // Get Color
    QBrush brush;
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QColor brushColor;
    UI::Tile tile = tiles.at(col + row * size);
    switch(tile.color)
    {
    case UI::TileColor::Blue:
        brushColor = Qt::blue;
        break;
    case UI::TileColor::LightBlue:
        brushColor = QColor(64, 64, 255);
        break;
    case UI::TileColor::Red:
        brushColor = Qt::red;
        break;
    case UI::TileColor::LightRed:
        brushColor = QColor(255,64,64);
        break;
    case UI::TileColor::Empty:
        brushColor = Qt::lightGray;
        break;
    case UI::TileColor::HighlightBlue:
        brushColor = QColor(64,64,255);
        brush.setStyle(Qt::BrushStyle::FDiagPattern);
        break;
    case UI::TileColor::HighlightRed:
        brushColor = QColor(255,64,64);
        brush.setStyle(Qt::BrushStyle::FDiagPattern);
        break;
    }

    brush.setColor(brushColor);
    painter.fillPath(path, brush);
    painter.strokePath(path, QPen(Qt::black));

    // Text
    QRect textRect = QFontMetrics(painter.font()).boundingRect(tile.text.c_str());
    painter.drawText(-0.5 * textRect.width(), textRect.height() * 0.25, tile.text.c_str()); // I don't know why *0.25 works, but it does

    painter.restore();
}

void HexGrid::drawBorder(QPainter &painter)
{
    QPointF upperLeft(0.134 * TILE_SIZE,TILE_SIZE * 0.5);
    QPointF upperRight((size + 1) * COL_GAP - 0.432 * TILE_SIZE, TILE_SIZE * 0.5);
    QPointF lowerLeft((size * 0.5) * COL_GAP + 0.268 * TILE_SIZE, (size - 1) * ROW_GAP + 1.5 * TILE_SIZE);
    QPointF lowerRight((1.5 * size + 1) * COL_GAP - 0.298 * TILE_SIZE, (size - 1) * ROW_GAP + 1.5 * TILE_SIZE);

    QPen blue(Qt::blue);
    QPen red(Qt::red);

    painter.setPen(red);
    painter.drawLine(upperLeft, upperRight);
    painter.drawLine(lowerLeft, lowerRight);
    painter.setPen(blue);
    painter.drawLine(upperLeft, lowerLeft);
    painter.drawLine(upperRight, lowerRight);
}

int HexGrid::getTileClicked(qreal xMousePos, qreal yMousePos)
{
    qreal minDistance = CLICK_MAX_DISTANCE;
    int tile = -1;
    for (auto i = 0; i < size; i++)
        for (auto j = 0; j < size; j++)
        {
            qreal testX, testY;
            getTilePosition(testX, testY, i, j);

            qreal deltaX = testX - xMousePos;
            qreal deltaY = testY - yMousePos;
            qreal dist = sqrt(deltaX * deltaX + deltaY * deltaY);
            if (dist < minDistance)
            {
                minDistance = dist;
                tile = i + j * size;
            }
        }

    return tile;
}

void HexGrid::getTilePosition(qreal &xPos, qreal &yPos, int col, int row)
{
    xPos = ((col % size) + (row * 0.5)) * COL_GAP + TILE_SIZE;;
    yPos = row * ROW_GAP + TILE_SIZE;
}

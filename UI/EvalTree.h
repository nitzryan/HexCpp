#ifndef EVALTREE_H
#define EVALTREE_H

#include <QWidget>
#include <QTreeWidget>
#include "UI.h"

class EvalTree : public QWidget
{
    Q_OBJECT
public:
    explicit EvalTree(QWidget *parent = nullptr);
    ~EvalTree();
public slots:
    void UpdateEvalTree(UI::TileTree tree);
signals:
    void BoardTiles(std::vector<UI::Tile> tiles);
    void EvalScore(float score);
private:
    UI::TileTree tree;
    QTreeWidget* treeWidget;

    enum class DataDisplayType
    {
        Score,
        HScore,
        Tile,
        NumChildren
    };
    DataDisplayType displayType = DataDisplayType::Score;

    void SendTileNode(std::shared_ptr<UI::TileNode> node);
    void DataDisplayTypeChanged(DataDisplayType type);
private slots:
    void TreeExpanded(QTreeWidgetItem* item);
    void TreeCollapsed(QTreeWidgetItem* item);
    void NodeSelected(QTreeWidgetItem* item);
};

#endif // EVALTREE_H

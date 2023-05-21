#include "EvalTree.h"
#include <QLayout>
#include <QRadioButton>

EvalTree::EvalTree(QWidget *parent)
    : QWidget{parent}
{
    treeWidget = new QTreeWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QRadioButton* scoreButton = new QRadioButton("Score");
    QRadioButton* hScoreButton = new QRadioButton("Heuristic Score");
    QRadioButton* tileButton = new QRadioButton("Tile ID");
    QRadioButton* numChildrenButton = new QRadioButton("Num Children");
    scoreButton->setChecked(true);
    layout->addWidget(scoreButton);
    layout->addWidget(hScoreButton);
    layout->addWidget(tileButton);
    layout->addWidget(numChildrenButton);

    connect(scoreButton, &QRadioButton::clicked, this, [this](){
        DataDisplayTypeChanged(DataDisplayType::Score);
    });
    connect(hScoreButton, &QRadioButton::clicked, this, [this](){
        DataDisplayTypeChanged(DataDisplayType::HScore);
    });
    connect(tileButton, &QRadioButton::clicked, this, [this](){
        DataDisplayTypeChanged(DataDisplayType::Tile);
    });
    connect(numChildrenButton, &QRadioButton::clicked, this, [this](){
        DataDisplayTypeChanged(DataDisplayType::NumChildren);
    });

    layout->addWidget(treeWidget);
    setLayout(layout);
}

EvalTree::~EvalTree()
{

}

void EvalTree::UpdateEvalTree(UI::TileTree t)
{
    tree = t;
    treeWidget->clear();
    QTreeWidgetItem* root = new QTreeWidgetItem(treeWidget);
    root->setText(0, "Root");
    root->setData(0, Qt::UserRole, QVariant::fromValue(t.root));
    if (t.root->children.size() != 0) {
        root->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    } else {
        root->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    }
    connect(treeWidget, &QTreeWidget::itemExpanded, this, &EvalTree::TreeExpanded);
    connect(treeWidget, &QTreeWidget::itemCollapsed, this, &EvalTree::TreeCollapsed);
    connect(treeWidget, &QTreeWidget::currentItemChanged, this, &EvalTree::NodeSelected);
    emit EvalScore(tree.root->score);
    SendTileNode(tree.root);
}

void EvalTree::SendTileNode(std::shared_ptr<UI::TileNode> node)
{
    // Get Colors
    auto tileColors = tree.tilesBeforeEval;
    auto parent = node->parent.lock();
    while (parent != nullptr) {
        if (parent->parent.lock() != nullptr) {
            tileColors.at(parent->tile) = parent->tileColor;
        }
        parent = parent->parent.lock();
    }
    if (node->parent.lock() != nullptr) {
        tileColors.at(node->tile) = UI::TileIsBlue(node->tileColor) ? UI::TileColor::HighlightBlue : UI::TileColor::HighlightRed;
    } else {
        tileColors.at(tree.lastMove) = !tree.lastMoveRed ? UI::TileColor::HighlightBlue : UI::TileColor::HighlightRed;
    }


    // Create tiles
    std::vector<UI::Tile> tiles;
    tiles.reserve(tileColors.size());
    for (auto& i : tileColors) {
        tiles.push_back({"", i});
    }
    for (auto& i : node->children) {
        switch(displayType) {
        case DataDisplayType::Tile:
            tiles.at(i->tile).text = QString::number(i->tile, 'd', 0).toStdString();
            break;
        case DataDisplayType::HScore:
            tiles.at(i->tile).text = QString::number(i->heuristicScore, 'f', 3).toStdString();
            break;
        case DataDisplayType::Score:
            tiles.at(i->tile).text = QString::number(i->score, 'f', 3).toStdString();
            break;
        case DataDisplayType::NumChildren:
            tiles.at(i->tile).text = QString::number(i->numChildren, 'd', 0).toStdString();
            break;
        }


    }

    emit BoardTiles(tiles);

}

void EvalTree::DataDisplayTypeChanged(DataDisplayType type)
{
    displayType = type;
    auto nodes = treeWidget->selectedItems();
    if (!nodes.isEmpty()) {
        auto node = nodes.first()->data(0, Qt::UserRole).value<std::shared_ptr<UI::TileNode>>();
        SendTileNode(node);
    }
}

void EvalTree::TreeExpanded(QTreeWidgetItem *item)
{
    if (item->childCount() != 0) {
        return;
    }

    std::shared_ptr<UI::TileNode> node = item->data(0, Qt::UserRole).value<std::shared_ptr<UI::TileNode>>();

    for (auto& i : node->children) {
        QTreeWidgetItem* child = new QTreeWidgetItem(item);
        child->setText(0, QString::number(i->tile));
        child->setData(0, Qt::UserRole, QVariant::fromValue(i));
        if (i->children.size() != 0) {
            child->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        } else {
            child->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
        }
        item->addChild(child);
    }
}

void EvalTree::TreeCollapsed(QTreeWidgetItem *item)
{
    auto children = item->takeChildren();
    for (auto& child : children) {
        delete child;
    }

    item->setExpanded(false);
}

void EvalTree::NodeSelected(QTreeWidgetItem *item)
{
    if (item == nullptr) {
        return;
    }
    std::shared_ptr<UI::TileNode> node = item->data(0, Qt::UserRole).value<std::shared_ptr<UI::TileNode>>();
    emit EvalScore(node->score);
    SendTileNode(node);
}

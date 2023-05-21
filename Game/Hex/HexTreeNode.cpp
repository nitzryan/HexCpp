#include "HexTreeNode.h"

HexTreeNode::HexTreeNode(float score, float heuristicScore, short tileIndex, Hex::Tile tile, std::shared_ptr<HexTreeNode> parent)
{
    this->score = score;
    this->heuristicScore = heuristicScore;
    this->tileIndex = tileIndex;
    this->tile = tile;
    this->parent = parent;
}

void HexTreeNode::AddChild(std::shared_ptr<HexTreeNode> child)
{
    children.push_back(child);
}

float HexTreeNode::GetScore() const
{
    return score;
}

float HexTreeNode::GetHeuristicScore() const
{
    return heuristicScore;
}

short HexTreeNode::GetTile() const
{
    return tileIndex;
}

Hex::Tile HexTreeNode::GetHexTile() const
{
    return tile;
}

const std::shared_ptr<HexTreeNode> HexTreeNode::GetParent() const
{
    return parent.lock();
}

const std::vector<std::shared_ptr<HexTreeNode> > HexTreeNode::GetChildren() const
{
    return children;
}

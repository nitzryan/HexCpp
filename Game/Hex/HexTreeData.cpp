#include "HexTreeData.h"

HexTreeData::HexTreeData(const std::vector<Hex::Tile> &currentPosTiles, const std::vector<std::unique_ptr<HexMove> > &moves, std::shared_ptr<HexTreeNode> root)
{
    tiles = currentPosTiles;
    for (auto& i : moves) {
        this->moves.push_back(std::make_unique<HexMove>(*i.get()));
    }
    this->root = root;
}

const std::vector<Hex::Tile> &HexTreeData::GetTiles() const
{
    return tiles;
}

std::vector<std::unique_ptr<HexMove> > HexTreeData::GetMoves() const
{
    std::vector<std::unique_ptr<HexMove>> result;
    for (auto& i : moves) {
        result.push_back(std::make_unique<HexMove>(i->IsRed(), i->GetTile(), false));
    }

    return result;
}

const std::shared_ptr<HexTreeNode> HexTreeData::GetRoot() const
{
    return root;
}

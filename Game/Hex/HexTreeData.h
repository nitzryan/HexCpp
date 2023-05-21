#ifndef HEXTREEDATA_H
#define HEXTREEDATA_H

#include <vector>
#include "HexMove.h"
#include "HexTreeNode.h"
#include "../AbstractTreeData.h"

class HexTreeData : public AbstractTreeData
{
public:
    HexTreeData(const std::vector<Hex::Tile>& currentPosTiles,
                const std::vector<std::unique_ptr<HexMove>>& moves,
                std::shared_ptr<HexTreeNode> root);
    const std::vector<Hex::Tile>& GetTiles() const;
    std::vector<std::unique_ptr<HexMove>> GetMoves() const;
    const std::shared_ptr<HexTreeNode> GetRoot() const;
private:
    std::vector<Hex::Tile> tiles;
    std::vector<std::unique_ptr<HexMove>> moves;
    std::shared_ptr<HexTreeNode> root;
};

#endif // HEXTREEDATA_H

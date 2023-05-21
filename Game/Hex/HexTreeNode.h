#ifndef HEXTREENODE_H
#define HEXTREENODE_H

#include <memory>
#include <vector>

namespace Hex {
    enum class Tile : char
    {
        Empty = 0,
        Red = 1,
        Blue = 2
    };
}

class HexTreeNode
{
public:
    HexTreeNode(float score,
                float heuristicScore,
                short tileIndex,
                Hex::Tile tile,
                std::shared_ptr<HexTreeNode> parent);
    
    void AddChild(std::shared_ptr<HexTreeNode> child);
    float GetScore() const;
    float GetHeuristicScore() const;
    short GetTile() const;
    Hex::Tile GetHexTile() const;
    const std::shared_ptr<HexTreeNode> GetParent() const;
    const std::vector<std::shared_ptr<HexTreeNode>> GetChildren() const;
private:
    float score;
    float heuristicScore;
    short tileIndex;
    Hex::Tile tile;
    std::vector<std::shared_ptr<HexTreeNode>> children;
    std::weak_ptr<HexTreeNode> parent;
};

#endif // HEXTREENODE_H

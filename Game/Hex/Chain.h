#ifndef CHAIN_H
#define CHAIN_H

#include "BitArray.h"
#include <vector>

class Chain
{
public:
    Chain(short tile, short size, bool isRed, BitArray* placedTiles);
    bool ShouldMerge(const Chain& other) const;
    void MergeWith(Chain* other);
    void OpponentTilePlaced(short move);
    bool Traversed() const;
    void SetTraversed(bool b) { traversed = b; }
    bool ShouldDelete() const;
    char GetChainLength() const;
    char GetMinRank() const { return minRank; }
    char GetMaxRank() const { return maxRank; }
    bool IsRed() const { return isRed; }
    size_t getRankedTilesLength() const { return rankedTiles.size(); }
    const BitArray* GetAdjacencies() const { return &adjacent; }
    short TemporaryFunction() const { return rankedTiles[0]; }
private:
    BitArray tiles;
    BitArray adjacent;
    char minRank, maxRank; // Ignors values that can't be moved forward from
    bool traversed = false;
    bool shouldDelete = false;
    bool isRed;
    std::vector<short> rankedTiles; // Tiles that contribute to the min/max rank
};

#endif // CHAIN_H

#include "Chain.h"

Chain::Chain(short tile, short size, bool isRed, BitArray* placedTiles)
{
    this->isRed = isRed;
    short size2 = size * size;
    tiles = BitArray(size2, tile);

    // Determine which tiles are adjacent to tile
    short col = tile % size;
    short row = tile / size;
    std::vector<short> adjTiles;
    adjTiles.reserve(6);
    if (col > 0) {
        adjTiles.push_back(tile - 1);
        if (row < size - 1) {
            adjTiles.push_back(tile - 1 + size);
        }
    }
    if (col < size - 1) {
        adjTiles.push_back(tile + 1);
        if (row > 0) {
            adjTiles.push_back(tile + 1 - size);
        }
    }
    if (row > 0) {
        adjTiles.push_back(tile - size);
    }
    if (row < size - 1) {
        adjTiles.push_back(tile + size);
    }

    adjacent = BitArray(size2, adjTiles);
    BitArray emptyTiles = BitArray::Not(placedTiles);
    adjacent = BitArray::And(&adjacent, &emptyTiles);

    // Set rank
    minRank = isRed ? static_cast<char>(row) : static_cast<char>(col);
    maxRank = minRank;

    maxRankedTiles = BitArray(size2, tile);
    minRankedTiles = BitArray(size2, tile);
}

bool Chain::ShouldMerge(const Chain &other) const
{
    BitArray tilesAndAdjacent = BitArray::And(&tiles, &other.adjacent);
    return tilesAndAdjacent.AtLeaseOneBit();
}

void Chain::MergeWith(Chain* other)
{
    // Set tiles and adjacencies
    tiles = BitArray::Or(&tiles, &other->tiles);
    auto tmp1 = BitArray::Or(&adjacent, &other->adjacent);
    auto tmp2 = BitArray::Not(&tiles);
    adjacent = BitArray::And(&tmp1, &tmp2);

    // Set rank
    minRank = std::min(minRank, other->minRank);
    maxRank = std::max(maxRank, other->maxRank);

    // Mark other for deletion, so that HexBoardTree knows to remove
    other->shouldDelete = true;

    minRankedTiles = BitArray::Or(&minRankedTiles, &other->minRankedTiles);
    maxRankedTiles = BitArray::Or(&maxRankedTiles, &other->maxRankedTiles);
}

void Chain::OpponentTilePlaced(short move)
{
    adjacent.UnsetBit(move);
}

bool Chain::Traversed() const
{
    return traversed;
}

bool Chain::ShouldDelete() const
{
    return shouldDelete;
}

char Chain::GetChainLength() const
{
    return maxRank - minRank;
}

void Chain::ApplyPoison(BitArray maxPoison, BitArray minPoison, const HexBoardHelper* helper)
{
    auto tmp = BitArray::Not(&maxPoison);
    maxRankedTiles = BitArray::And(&maxRankedTiles, &tmp);
    tmp = BitArray::Not(&minPoison);
    minRankedTiles = BitArray::And(&minRankedTiles, &tmp);

    helper->CalculateRank(maxRankedTiles, minRankedTiles, minRank, maxRank, isRed);
}

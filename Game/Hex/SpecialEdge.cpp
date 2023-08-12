#include "SpecialEdge.h"

SpecialEdge::SpecialEdge(Chain* c, short edgeRank, BitArray edgeTiles)
{
	chain = c;
	rank = edgeRank;
	edgeTemplate = edgeTiles;
	broken = false;
}

short SpecialEdge::GetRank() const
{
	return rank;
}

Chain* SpecialEdge::GetChain() const
{
	return chain;
}

void SpecialEdge::RemapAddresses(const Chain* old, Chain* current)
{
	if (chain == old) {
		chain = current;
	}
}

void SpecialEdge::OpponentMoveMade(short tile)
{
	if (edgeTemplate.BitIsSet(tile)) {
		broken = true;
	}
}

bool SpecialEdge::IsBroken() const
{
	return broken;
}

bool SpecialEdge::ShouldBeEdge1(short tile, bool isRed, const BitArray* notEnemyTiles, BitArray& edgeTiles, const HexBoardHelper* helper)
{
	return helper->IsEdge1(tile, isRed, notEnemyTiles, edgeTiles);
}


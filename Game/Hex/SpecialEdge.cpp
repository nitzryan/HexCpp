#include "SpecialEdge.h"
#include <algorithm>

SpecialEdge::SpecialEdge(const Chain* c, short move, short edgeRank, const HexBoardHelper* helper, const BitArray* notOpposingTiles)
{
	chain = c;
	rank = edgeRank;
	numTiles = helper->GetNumberOfTiles();
	templates = helper->GetSpecialEdges(move, c->IsRed(), notOpposingTiles);
}

short SpecialEdge::GetRank() const
{
	return rank;
}

const Chain* SpecialEdge::GetChain() const
{
	return chain;
}

void SpecialEdge::RemapAddresses(const Chain* old, const Chain* current)
{
	if (chain == old) {
		chain = current;
	}
}

void SpecialEdge::OpponentMoveMade(short tile)
{
	auto end = (std::remove_if(templates.begin(), templates.end(), [tile](const BitArray& ba) { return ba.BitIsSet(tile); }));
	templates.erase(end, templates.end());
}

bool SpecialEdge::IsBroken() const
{
	return templates.empty();
}

BitArray SpecialEdge::GetTiles() const
{
	BitArray ba(numTiles);
	for (auto& t : templates)
		ba = BitArray::Or(&ba, &t);

	return ba;
}


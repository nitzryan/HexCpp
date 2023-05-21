#include "Edge.h"

Edge::Edge(Chain* c, char edgeRank, bool red)
{
	chain = c;
	rank = edgeRank;
	isRed = red;
}

char Edge::GetRank() const
{
	return rank;
}

Chain* Edge::GetChain() const
{
	return chain;
}

bool Edge::IsStillEdge(const HexBoardHelper* helper) const
{
	return Edge::ShouldBeEdge(chain, rank, helper);
}

void Edge::RemapAddresses(Chain* old, Chain* current)
{
	if (chain == old) {
		chain = current;
	}
}

bool Edge::ShouldBeEdge(Chain* chain, char rank, const HexBoardHelper* helper)
{
	if (chain->IsRed()) {
		return BitArray::And(chain->GetAdjacencies(), helper->GetRankBitArrayRed(rank)).MoreThanOneBit();
	}
	return BitArray::And(chain->GetAdjacencies(), helper->GetRankBitArrayBlue(rank)).MoreThanOneBit();
}

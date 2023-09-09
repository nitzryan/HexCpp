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

const Chain* Edge::GetChain() const
{
	return chain;
}

bool Edge::IsStillEdge(const HexBoardHelper* helper) const
{
	return Edge::ShouldBeEdge(chain, rank, helper);
}

void Edge::RemapAddresses(const Chain* old, const Chain* current)
{
	if (chain == old) {
		chain = current;
	}
}

bool Edge::ShouldBeEdge(const Chain* chain, char rank, const HexBoardHelper* helper)
{
	if ((rank == 0 && chain->GetMinRank() == 0) || (rank == helper->GetMaxRank() && chain->GetMaxRank() == helper->GetMaxRank())) { // Not edge since already connected
		return false;
	}
	if (chain->IsRed()) {
		return BitArray::And(chain->GetAdjacencies(), helper->GetRankBitArrayRed(rank)).MoreThanOneBit();
	}
	return BitArray::And(chain->GetAdjacencies(), helper->GetRankBitArrayBlue(rank)).MoreThanOneBit();
}

const BitArray Edge::GetTiles(const HexBoardHelper* helper) const
{
	if (chain->IsRed()) {
		return BitArray::And(chain->GetAdjacencies(), helper->GetRankBitArrayRed(rank));
	}
	return BitArray::And(chain->GetAdjacencies(), helper->GetRankBitArrayBlue(rank));
}

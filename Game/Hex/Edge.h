#ifndef EDGE_H
#define EDGE_H

#include "BitArray.h"
#include "Chain.h"
#include "HexBoardHelper.h"

class Edge {
public:
	Edge(Chain* c, char edgeRank, bool red);
	char GetRank() const;
	const Chain* GetChain() const;
	bool IsStillEdge(const HexBoardHelper* helper) const;
	void RemapAddresses(const Chain* old, const Chain* current);
	static bool ShouldBeEdge(const Chain* chain, char rank, const HexBoardHelper* helper);
	const BitArray GetTiles(const HexBoardHelper* helper) const;
private:
	const Chain* chain;
	char rank;
	bool isRed;
};

#endif
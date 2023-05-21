#ifndef EDGE_H
#define EDGE_H

#include "BitArray.h"
#include "Chain.h"
#include "HexBoardHelper.h"

class Edge {
public:
	Edge(Chain* c, char edgeRank, bool red);
	char GetRank() const;
	Chain* GetChain() const;
	bool IsStillEdge(const HexBoardHelper* helper) const;
	void RemapAddresses(Chain* old, Chain* current);
	static bool ShouldBeEdge(Chain* chain, char rank, const HexBoardHelper* helper);
private:
	Chain* chain;
	char rank;
	bool isRed;
};

#endif
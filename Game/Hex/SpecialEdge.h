#pragma once

#include "BitArray.h"
#include "Chain.h"
#include "HexBoardHelper.h"

class SpecialEdge {
public:
	SpecialEdge(Chain* c, short edgeRank, BitArray edgeTiles);
	short GetRank() const;
	Chain* GetChain() const;
	void RemapAddresses(const Chain* old, Chain* current);
	void OpponentMoveMade(short tile);
	bool IsBroken() const;
	static bool ShouldBeEdge1(short tile, bool isRed, const BitArray* notEnemyTiles, BitArray& edgeTiles, const HexBoardHelper* helper);
private:
	bool broken;
	short rank;
	Chain* chain;
	BitArray edgeTemplate;
};
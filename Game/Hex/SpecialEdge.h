#pragma once

#include "BitArray.h"
#include "Chain.h"
#include "HexBoardHelper.h"

class SpecialEdge {
public:
	SpecialEdge(const Chain* c, short move, short edgeRank, const HexBoardHelper* helper, const BitArray* notOpposingTiles);
	short GetRank() const;
	const Chain* GetChain() const;
	void RemapAddresses(const Chain* old, const Chain* current);
	void OpponentMoveMade(short tile);
	bool IsBroken() const;
	BitArray GetTiles() const;
	size_t numTemplates() { return templates.size(); }
private:
	short rank;
	short numTiles;
	const Chain* chain;

	std::vector<BitArray> templates;
};
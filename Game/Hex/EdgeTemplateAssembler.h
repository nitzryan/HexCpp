#pragma once

#include "BitArray.h"
#include <vector>
#include "HexBoardHelper.h"

struct RowCol {
	short row;
	short col;
};

class EdgeTemplateAssembler
{
public:
	static std::map<Key1, BitArray> CreateTemplateOneTile(short size, std::vector<RowCol> emptyTiles);
};
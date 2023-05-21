#ifndef HEXBOARDHELPER_H
#define HEXBOARDHELPER_H

#include "BitArray.h"
#include <vector>

class HexBoardHelper {
public:
	HexBoardHelper(short size);
	const BitArray* GetRankBitArrayRed(short rank) const;
	const BitArray* GetRankBitArrayBlue(short rank) const;
private:
	std::vector<BitArray> redRankArray, blueRankArray;
	short size;
};

#endif

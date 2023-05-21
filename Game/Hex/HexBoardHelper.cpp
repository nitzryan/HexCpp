#include "HexBoardHelper.h"

HexBoardHelper::HexBoardHelper(short s) {
	size = s;
	redRankArray.reserve(size);
	blueRankArray.reserve(size);

	// Red Ranks
	for (short i = 0; i < size; i++) {
		std::vector<short> tiles;
		for (short j = 0; j < size; j++) {
			tiles.push_back(i * size + j);
		}
		redRankArray.push_back(BitArray(size * size, tiles));
	}

	// Blue Ranks
	// Red Ranks
	for (short i = 0; i < size; i++) {
		std::vector<short> tiles;
		for (short j = 0; j < size; j++) {
			tiles.push_back(i + j * size);
		}
		blueRankArray.push_back(BitArray(size * size, tiles));
	}
}

const BitArray* HexBoardHelper::GetRankBitArrayRed(short rank) const {
	return &redRankArray.at(rank);
}

const BitArray* HexBoardHelper::GetRankBitArrayBlue(short rank) const {
	return &blueRankArray.at(rank);
}
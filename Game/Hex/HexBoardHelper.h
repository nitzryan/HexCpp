#ifndef HEXBOARDHELPER_H
#define HEXBOARDHELPER_H

#include "BitArray.h"
#include <vector>
#include <map>

struct Key1 {
	short tile;
	bool red;
	bool operator<(const Key1& other) const {
		if (red && !other.red)
			return true;
		if (!red && other.red)
			return false;
		return tile > other.tile;
	}
};

class HexBoardHelper {
public:
	HexBoardHelper(short size);
	const BitArray* GetRankBitArrayRed(short rank) const;
	const BitArray* GetRankBitArrayBlue(short rank) const;
	bool IsEdge1(short tile, bool isRed, const BitArray* notOpposingTiles, BitArray& templateTiles) const;
	void CalculateRank(const BitArray& maxTiles, const BitArray& minTiles, char& minRank, char& maxRank, bool isRed) const;
private:
	std::vector<BitArray> redRankArray, blueRankArray;
	std::map<Key1, std::vector<BitArray>> oneTileEdges;
	static std::map<Key1, std::vector<BitArray>> InitializeMap(short size);

	short size;
};

#endif

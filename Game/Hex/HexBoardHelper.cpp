#include "HexBoardHelper.h"
#include <array>
#include "EdgeTemplateAssembler.h"

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

	oneTileEdges = InitializeMap(size);
}

const BitArray* HexBoardHelper::GetRankBitArrayRed(short rank) const {
	return &redRankArray.at(rank);
}

const BitArray* HexBoardHelper::GetRankBitArrayBlue(short rank) const {
	return &blueRankArray.at(rank);
}

bool HexBoardHelper::IsEdge1(short tile, bool isRed, const BitArray* notOpposingTiles, BitArray& templateTiles) const
{
	Key1 key;
	key.tile = tile;
	key.red = isRed;

	auto it = oneTileEdges.find(key);
	if (it == oneTileEdges.end())
		return false;

	// Potential edge was found
	std::vector<BitArray> templateVector = it->second;
	for (auto& v : templateVector) {
		auto notArray = BitArray::Not(notOpposingTiles);
		auto tmp = BitArray::And(&notArray, &v);
		if (BitArray::Equal(&tmp, &v))
		{
			templateTiles = v;
			return true;
		}
	}

	return false;
}

void HexBoardHelper::CalculateRank(const BitArray& maxTiles, const BitArray& minTiles, char& minRank, char& maxRank, bool isRed) const
{
	minRank = (char)size;
	maxRank = 0;

	auto TileInRank = [](const BitArray& test, const BitArray& tiles) {
		BitArray merged = BitArray::And(&tiles, &test);
		return merged.AtLeaseOneBit();
	};

	const std::vector<BitArray>* arr;
	if (isRed) {
		arr = &redRankArray;
	}
	else {
		arr = &blueRankArray;
	}

	for (char i = 0; i < arr->size(); i++)
	{
		if (TileInRank(arr->at(i), minTiles))
		{
			minRank = i;
			break;
		}
	}

	for (char i = (char)(arr->size() - 1); i >= 0; i--) 
	{
		if (TileInRank(arr->at(i), maxTiles))
		{
			maxRank = i;
			break;
		}
	}
}

std::map<Key1, std::vector<BitArray>> HexBoardHelper::InitializeMap(short size)
{
	std::map<Key1, std::vector<BitArray>> result;
	// 3rd row edge templates
	auto t1 = EdgeTemplateAssembler::CreateTemplateOneTile(size, { {0,1},{1,-1},{1,0},{1,1},{2,-2},{2,-1},{2,0},{2,1} });
	auto t2 = EdgeTemplateAssembler::CreateTemplateOneTile(size, { {0,-1},{1,-2},{1,-1},{1,0},{2,-3},{2,-2},{2,-1},{2,0} });
	for (const auto& i : t1) {
		result.insert(std::pair(i.first, std::vector<BitArray>{ i.second }));
	}
	for (const auto& i : t2) {
		auto it = result.find(i.first);
		if (it == result.end())
			result.insert(std::pair(i.first, std::vector<BitArray>{ i.second }));
		else
			result.at(i.first).push_back(i.second);
	}

	return result;
}
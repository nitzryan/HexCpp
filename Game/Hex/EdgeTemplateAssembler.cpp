#include "EdgeTemplateAssembler.h"

std::map<Key1, BitArray> EdgeTemplateAssembler::CreateTemplateOneTile(short size, std::vector<RowCol> emptyTiles)
{
	// Find required space for template
	short minRow = 0, maxRow = 0, minCol = 0, maxCol = 0;
	for (auto& i : emptyTiles) {
		if (i.row < minRow)
			minRow = i.row;
		else if (i.row > maxRow)
			maxRow = i.row;
		if (i.col < minCol)
			minCol = i.col;
		else if (i.col > maxCol)
			maxCol = i.col;
	}

	std::map<Key1, BitArray> result;

	// Red top templates
	for (short col = maxCol; col < size + minCol; col++) {
		std::vector<short> tls;
		Key1 key;
		key.red = true;
		key.tile = size * maxRow + col;
		tls.reserve(emptyTiles.size());
		for (auto& i : emptyTiles) {
			tls.push_back(key.tile - i.col - size * i.row);
		}

		result.insert(std::make_pair(key, BitArray(size * size, tls)));
	}

	// Red bot templates
	for (short col = -minCol; col < size - maxCol; col++) {
		std::vector<short> tls;
		Key1 key;
		key.red = true;
		key.tile = size * (size - maxRow - 1) + col;
		tls.reserve(emptyTiles.size());
		for (auto& i : emptyTiles) {
			tls.push_back(key.tile + i.col + size * i.row);
		}

		result.insert(std::make_pair(key, BitArray(size * size, tls)));
	}

	// Blue left templates
	for (short row = maxRow + minCol; row < size - maxRow - maxCol; row++) {
		std::vector<short> tls;
		Key1 key;
		key.red = false;
		key.tile = maxRow + row * size;
		tls.reserve(emptyTiles.size());
		for (auto& i : emptyTiles) {
			tls.push_back(key.tile + i.row * (size - 1) + i.col * size);
		}

		result.insert(std::make_pair(key, BitArray(size * size, tls)));
	}

	// Blue right templates
	short s = size * size - 1;
	for (short row = maxRow + minCol; row < size - maxRow - maxCol; row++) {
		std::vector<short> tls;
		Key1 key;
		key.red = false;
		key.tile = s - (maxRow + row * size);
		tls.reserve(emptyTiles.size());
		for (auto& i : emptyTiles) {
			tls.push_back(key.tile - i.row * (size - 1) - i.col * size);
		}

		result.insert(std::make_pair(key, BitArray(size * size, tls)));
	}

	return result;
}

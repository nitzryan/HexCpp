#include "Poison.h"

Poison::Poison(short size, bool red)
{
	short numTiles = size * size;
	maxPoison = BitArray(numTiles);
	minPoison = BitArray(numTiles);
	if (red) {
		
		maxAdj = BitArray(numTiles);
		minAdj = BitArray(numTiles);
		for (short i = 0; i < size; i++) {
			minAdj.SetBit(size * (i + 1) - 1);
			maxAdj.SetBit(size * i);
		}
	}
	else {
		maxAdj = BitArray(numTiles);
		minAdj = BitArray(numTiles);
		for (short i = 0; i < size; i++) {
			minAdj.SetBit(size * (size - 1) + i);
			maxAdj.SetBit(i);
		}
	}
}

Poison::Poison(const Poison& p)
{
	maxPoison = p.maxPoison;
	minPoison = p.minPoison;
	maxAdj = p.maxAdj;
	minAdj = p.minAdj;
}

void Poison::PlaceTile(short tile, short size, bool isRed)
{
	short row = tile / size;
	short col = tile % size;
	if (isRed) {
		if (col != 0)
		{
			PlaceMaxRed(tile - 1, size);
			if (row != (size - 1))
				PlaceMaxRed(tile + size - 1, size);
		}

		if (col != (size - 1))
		{
			PlaceMinRed(tile + 1, size);
			if (row != 0)
				PlaceMinRed(tile - size + 1, size);
		}
	}
	else {
		if (row != 0)
		{
			PlaceMaxBlue(tile - size, size);
			if (col != (size - 1))
				PlaceMaxBlue(tile - size + 1, size);
		}

		if (row != (size - 1))
		{
			PlaceMinBlue(tile + size, size);
			if (col != 0)
				PlaceMinBlue(tile + size - 1, size);
		}
	}
}

void Poison::PlaceMaxRed(short tile, short size)
{
	if (maxAdj.BitIsSet(tile))
	{
		maxPoison.SetBit(tile);
		short row = tile / size;
		short col = tile % size;
		if (col != 0) {
			PlaceMaxRed(tile - 1, size);
			if (row != (size - 1))
				PlaceMaxRed(tile + size - 1, size);
		}
	}
	else {
		maxAdj.SetBit(tile);
	}
}

void Poison::PlaceMinRed(short tile, short size)
{
	if (minAdj.BitIsSet(tile))
	{
		minPoison.SetBit(tile);
		short row = tile / size;
		short col = tile % size;
		if (col != (size - 1)) {
			PlaceMinRed(tile + 1, size);
			if (row != 0)
				PlaceMinRed(tile - size + 1, size);
		}
	}
	else {
		minAdj.SetBit(tile);
	}
}

void Poison::PlaceMaxBlue(short tile, short size)
{
	if (maxAdj.BitIsSet(tile))
	{
		maxPoison.SetBit(tile);
		short row = tile / size;
		short col = tile % size;
		if (row != 0) {
			PlaceMaxBlue(tile - size, size);
			if (col != (size - 1))
				PlaceMaxBlue(tile - size + 1, size);
		}
	}
	else {
		maxAdj.SetBit(tile);
	}
}

void Poison::PlaceMinBlue(short tile, short size)
{
	if (minAdj.BitIsSet(tile))
	{
		minPoison.SetBit(tile);
		short row = tile / size;
		short col = tile % size;
		if (row != (size - 1)) {
			PlaceMinBlue(tile + size, size);
			if (col != 0)
				PlaceMinBlue(tile + size - 1, size);
		}
	}
	else {
		minAdj.SetBit(tile);
	}
}

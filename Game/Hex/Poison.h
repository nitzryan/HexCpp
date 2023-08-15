#pragma once

#include "BitArray.h"

class Poison
{
public:
	Poison() {}
	Poison(short size, bool red);
	Poison(const Poison& p);
	const BitArray* GetMax() const { return &maxPoison; }
	const BitArray* GetMin() const { return &minPoison; }
	const BitArray* GetMaxAdj() const { return &maxAdj; }
	const BitArray* GetMinAdj() const { return &minAdj; }
	void PlaceTile(short tile, short size, bool isRed);
private:
	BitArray maxPoison, minPoison, maxAdj, minAdj;
	void PlaceMaxRed(short tile, short size);
	void PlaceMinRed(short tile, short size);
	void PlaceMaxBlue(short tile, short size);
	void PlaceMinBlue(short tile, short size);
};
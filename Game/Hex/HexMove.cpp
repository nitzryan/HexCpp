#include "HexMove.h"



HexMove::HexMove(bool isRed, short tile, bool isHumanMove)
{
    this->tile = tile;
    this->isRed = isRed;
    this->humanMove = isHumanMove;
}

HexMove::HexMove(const HexMove& move)
{
    this->tile = move.tile;
    this->isRed = move.isRed;
    this->humanMove = move.humanMove;
}

std::string HexMove::GetNotation() const
{
    if (isRed) {
        return "R:" + std::to_string(tile);
    }
    return "B:" + std::to_string(tile);
}

std::unique_ptr<AbstractMove> HexMove::GetCopy() const
{
    return std::make_unique<HexMove>(*this);
}

short HexMove::GetTile() const
{
    return tile;
}

bool HexMove::IsRed() const
{
    return isRed;
}

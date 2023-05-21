#ifndef HEXMOVE_H
#define HEXMOVE_H

#include "../AbstractMove.h"

class HexMove : public AbstractMove
{
public:
    /**
     * @brief HexMove
     * @param isRed
     * @param tile
     */
    HexMove(bool isRed, short tile, bool isHumanMove);
    /**
     * @brief HexMove
     * @param move
     */
    HexMove(const HexMove& move);
    /**
     * @brief GetNotation
     * @return Notation of Move
     */
    std::string GetNotation() const override;
    /**
    * @brief Gets a deep copy of the move
    * @return Deep copy of the move
    */
    std::unique_ptr<AbstractMove> GetCopy() const;
    short GetTile() const;
    bool IsRed() const;
private:
    short tile;
    bool isRed;
};

#endif // HEXMOVE_H

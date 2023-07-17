#ifndef HEXGAME_H
#define HEXGAME_H

#include "../AbstractGame.h"
#include "HexWeights.h"
#include "HexBoard.h"

class HexGame : public AbstractGame
{
public:
    HexGame(int size, float time, std::unique_ptr<HexWeights> p1AI, std::unique_ptr<HexWeights> p2AI);
    ~HexGame();
    bool NextMoveIsRed() const;
    std::unique_ptr<AbstractBoard> GetBoard(int size, AbstractWeights* weights, std::vector<std::unique_ptr<AbstractMove>> moves) const override;
private:

};

#endif // HEXGAME_H

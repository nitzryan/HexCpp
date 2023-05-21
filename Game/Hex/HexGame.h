#ifndef HEXGAME_H
#define HEXGAME_H

#include "../AbstractGame.h"
#include "HexWeights.h"

class HexGame : public AbstractGame
{
public:
    HexGame(int size, float time, std::unique_ptr<HexWeights> p1AI, std::unique_ptr<HexWeights> p2AI);
    ~HexGame();
    bool NextMoveIsRed() const;
private:

};

#endif // HEXGAME_H

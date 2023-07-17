#ifndef HEXAI_H
#define HEXAI_H

#include "../AbstractAI.h"
#include "HexBoard.h"
#include "HexWeights.h"
#include "HexTreeData.h"
#include <memory>

class HexAI : public AbstractAI
{
public:
    /**
    * @brief Constructor for a player in Hex
    * @param t Time in seconds to make each move
    * @param w Weights to use in making evaluation
    * @param size the size of the board
    */
    HexAI(float t, std::unique_ptr<HexWeights> w, int size);
    ~HexAI();
private:
    std::unique_ptr<HexWeights> weights;
};

#endif // HEXAI_H

#include "HexAI.h"
#include "HexWeights.h"

HexAI::HexAI(float t, std::unique_ptr<HexWeights> w, int size)
{
    weights = std::move(w);
    time = t;
    board = std::make_unique<HexBoard>(size, weights.get());
}

HexAI::~HexAI()
{

}

#include "HexWeights.h"

HexWeights::HexWeights():
    DepthPenalty(0.05f),
    CenterPenalty(0.02f),
    ChainLengthFactor(0.07f),
    TemplateLengthFactor(0.10f)
{

}

std::unique_ptr<AbstractWeights> HexWeights::GetWeights() const
{
    return nullptr;
}

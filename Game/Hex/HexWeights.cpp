#include "HexWeights.h"

HexWeights::HexWeights(int size):
    DepthPenalty(0.05f),
    CenterPenalty(0.02f),
    TempoBias(0.10f)
{
    ChainLengthFactor.reserve(size);
    TemplateLengthFactor.reserve(size);

    float chainInc = 0.01f;
    float templateInc = 0.04f;

    float c = 0;
    float t = 0;
    for (int i = 0; i < size - 1; i++) {
        c += chainInc;
        t += templateInc;
        ChainLengthFactor.push_back(c);
        TemplateLengthFactor.push_back(t);
        chainInc += 0.01f;
        templateInc += 0.02f;
    }

    ChainLengthFactor.push_back(1000);
    TemplateLengthFactor.push_back(100);
}

std::unique_ptr<AbstractWeights> HexWeights::GetWeights() const
{
    return nullptr;
}

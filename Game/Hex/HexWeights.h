#ifndef HEXWEIGHTS_H
#define HEXWEIGHTS_H

#include "../AbstractWeights.h"
#include <vector>

class HexWeights : public AbstractWeights
{
public:
    HexWeights(int size);
    std::unique_ptr<AbstractWeights> GetWeights() const override;

    float DepthPenalty;
    float CenterPenalty;
    std::vector<float> ChainLengthFactor;
    std::vector<float> TemplateLengthFactor;
    float TempoBias;
};

#endif // HEXWEIGHTS_H

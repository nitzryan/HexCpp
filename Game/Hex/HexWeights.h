#ifndef HEXWEIGHTS_H
#define HEXWEIGHTS_H

#include "../AbstractWeights.h"

class HexWeights : public AbstractWeights
{
public:
    HexWeights();
    std::unique_ptr<AbstractWeights> GetWeights() const override;

    float DepthPenalty;
    float CenterPenalty;
    float ChainLengthFactor;
    float TemplateLengthFactor;
    float TempoBias;
};

#endif // HEXWEIGHTS_H

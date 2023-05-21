#ifndef ABSTRACTWEIGHTS_H
#define ABSTRACTWEIGHTS_H

#include <memory>

class AbstractWeights
{
public:
    virtual ~AbstractWeights() {}
    /**
     * @brief GetWeights
     * @return A copy of the weights
     */
    virtual std::unique_ptr<AbstractWeights> GetWeights() const = 0;
};

#endif // ABSTRACTWEIGHTS_H

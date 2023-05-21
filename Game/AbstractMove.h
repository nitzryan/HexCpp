#ifndef ABSTRACTMOVE_H
#define ABSTRACTMOVE_H

#include <string>
#include <memory>

class AbstractMove
{
public:
    virtual ~AbstractMove() {}
    /**
     * @brief GetNotation
     * @return Notation of the move
     */
    virtual std::string GetNotation() const = 0;
    /**
     * @brief IsHumanMove
     * @return
     */
    bool IsHumanMove() const {return humanMove;}
    /**
    * @brief Gets a deep copy of the move
    * @return Deep copy of the move
    */
    virtual std::unique_ptr<AbstractMove> GetCopy() const = 0;
protected:
    bool humanMove;
};

#endif // ABSTRACTMOVE_H

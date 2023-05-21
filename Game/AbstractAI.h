#ifndef ABSTRACTAI_H
#define ABSTRACTAI_H

#include "AbstractBoard.h"
#include "AbstractWeights.h"
#include "AbstractMove.h"
#include <memory>

class AbstractAI
{
public:
    /**
     * @brief GetBestMove
     * @return Best Move for current player
     */
    std::unique_ptr<AbstractMove> GetBestMove() const;
    /**
     * @brief EvalBoard
     * @return The evaluations for the board in the current state
     */
    std::unique_ptr<AbstractTreeData> EvalBoard() const;

    /**
    * @brief Updates the board state to include the latest move
    * @param move The move that needs to be made
    */
    void MakeMove(AbstractMove* move);

protected:
    std::unique_ptr<AbstractWeights> weights;
    std::unique_ptr<AbstractBoard> board;
    float time = 1.0f;
};

#endif // ABSTRACTAI_H

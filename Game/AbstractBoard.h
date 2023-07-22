#ifndef ABSTRACTBOARD_H
#define ABSTRACTBOARD_H

#include <memory>
#include <vector>
#include "AbstractMove.h"
#include "AbstractTreeData.h"
#include "AbstractWeights.h"

class AbstractBoard
{
public:
    virtual ~AbstractBoard() {};
    /**
     * @brief GetBoard
     * Gets the board state, either 1 layer deep or all layers
     * @param expanded Whether evaluation tree should be included
     * @return Copy of current board state
     */
    virtual std::unique_ptr<AbstractBoard> GetBoard(bool expanded) const = 0;
    /**
     * @brief SetWeights
     * Determines the heuristic function for determining the value at each position
     * @param weights heuristic weights
     */
    virtual void MakeMove(const AbstractMove* move) = 0;
    /**
     * @brief ExpandBoardEval
     * Runs the heuristic function recursively on the board to find the best move
     * @param time Seconds to run until function returns
     * @param weights Weights for Heuristic Functions
     */
    virtual void ExpandBoardEval(float time) = 0;
    /**
     * @brief PruneBoard
     * Removes all values of the evaluation function except at the initial step
     */
    virtual void PruneBoard() = 0;
    /**
    * @brief RemoveChildren
    */
    virtual void Clear() = 0;
    /**
     * @brief GetBestMove
     * @return Best move available, as evaluated by heuristic function
     */
    virtual std::unique_ptr<AbstractMove> GetBestMove() const = 0;
    /**
     * @brief GameIsOver
     * @return If Game has ended, either due to win state or lack of moves
     */
    virtual bool GameIsOver() const = 0;
    /**
     * @brief GetNextPlayerToMove
     * @return
     */
    virtual short GetNextPlayerToMove() const = 0;
    /**
     * @brief IsMoveLegal
     * @param move
     * @return
     */
    virtual bool IsMoveLegal(const std::unique_ptr<AbstractMove>& move) const = 0;
    /**
     * @brief GetTreeData
     * @return
     */
    virtual std::unique_ptr<AbstractTreeData> GetTreeData() const = 0;
    /**
     * @brief SetChosenMove
     * @param move
     */
    virtual std::unique_ptr<AbstractMove> GetChosenMove() const = 0;
    /**
     * @brief SetWeights
     * @param weights
     */
    virtual void SetWeights(AbstractWeights* weights) = 0;
    /**
     * @brief GetMoves
     * @return
     */
    virtual std::vector<std::unique_ptr<AbstractMove>> GetMoves() const = 0;
    virtual int GetSize() const = 0;
};

#endif // ABSTRACTBOARD_H

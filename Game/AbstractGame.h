#ifndef ABSTRACTGAME_H
#define ABSTRACTGAME_H

#include "AbstractAI.h"
#include "AbstractBoard.h"
#include "AbstractMove.h"
#include "AbstractTreeData.h"

#include <memory>
#include <vector>
#include <mutex>
#include <future>
#include <functional>

class AbstractGame
{
public:
    /**
     * @brief AbstractGame
     * @param board
     */
    AbstractGame(std::unique_ptr<AbstractBoard> board);
    /**
     * @brief ~AbstractGame
     */
    virtual ~AbstractGame() {};

    /**
     * @brief AddBoardstateCallback
     * This function will set the callback function that will
     * Be called whenever the Board is updated
     * @param callback Function that will take an updated BoardState
     */
    virtual void AddBoardstateCallback(std::function<void(std::unique_ptr<AbstractTreeData>, std::unique_ptr<AbstractMove>)> callback);

    /**
     * @brief MakeMove
     * Takes a move from a human player
     * If this function causes an AIPlayer to make a move, it will do this in a
     * Seperate thread while this function will immediately return
     * @param move The move the player requested
     * @param board The updated board.  Nullptr if move was made by a human
     */
    void MakeMove(std::unique_ptr<AbstractMove> move);

    /**
     * @brief StepBack
     * Moves Board State back 1 move, does not commit current state
     * Will not move board state past first move
     */
    void StepBack();

    /**
     * @brief StepForward
     * Moves Board State forward 1 move, does not commit current state
     * Will not move board state past last move made
     */
    void StepForward();

    /**
     * @brief StepEnd
     * Moves Board State to the last state, does not commit current state
     */
    void StepEnd();

    /**
     * @brief StepStart
     * Moves Board State to starting state, does not commit current state
     */
    void StepStart();

    /**
     * @brief SetStep
     * Moves board state to state indicated by step
     * Will check that step is within bounds
     * @param step Game Step to move to, 0 indexed
     */
    void SetStep(size_t step);

    /**
     * @brief StartFromCurrentState
     * Commits the current Board State
     * Causes all board states after this state to be removed
     * If next move is by an AI, it will immediately start making its move
     */
    void StartFromCurrentState();
    /**
     * @brief Evaluate board
     */
    void EvaluateBoard(std::unique_ptr<AbstractWeights> weights, float time);
    /**
    */
    virtual std::unique_ptr<AbstractBoard> GetBoard(int size, AbstractWeights* weights, std::vector<std::unique_ptr<AbstractMove>> moves) const = 0;
protected:
    std::vector<std::unique_ptr<AbstractAI>> players; // Human players are nullptr, but need to be in vector
    std::vector<std::unique_ptr<AbstractAI>>::iterator currentPlayer;
    std::unique_ptr<AbstractBoard> GetCurrentBoardState() const;
    std::unique_ptr<AbstractBoard> board;
    int size;
private:
    std::mutex mutex;
    std::future<void> MakeMoveAsync;
    std::vector<std::unique_ptr<AbstractBoard>> boardStates;
    size_t uncommitedBoardStateId = 0;
    std::function<void(std::unique_ptr<AbstractTreeData>, std::unique_ptr<AbstractMove>)> callback = nullptr;
};

#endif // ABSTRACTGAME_H

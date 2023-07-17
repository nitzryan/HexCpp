#include "AbstractGame.h"
#include <assert.h>


AbstractGame::AbstractGame(std::unique_ptr<AbstractBoard> b)
{
    board = b->GetBoard(false);
    boardStates.emplace_back(std::move(b));
}

void AbstractGame::AddBoardstateCallback(std::function<void (std::unique_ptr<AbstractTreeData>, std::unique_ptr<AbstractMove>)> c)
{
    this->callback = c;
}

void AbstractGame::MakeMove(std::unique_ptr<AbstractMove> m)
{
    // Determine that move is legal, as human may attempt invalid moves (computers are perfect and have no bugs)
    // Also ensure that a human cannot make a move in a finished game
    if (m != nullptr && !boardStates.back()->IsMoveLegal(m) || boardStates.back()->GameIsOver()) {
        return;
    }

    // Ensure that previous async function had chance to run
    std::lock_guard<std::mutex> lock(mutex);

    // Return if another instance is running
    if (MakeMoveAsync.valid() && MakeMoveAsync.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
        return;
    }

    MakeMoveAsync = std::async(std::launch::async, [this, move = std::move(m)]() mutable {
        do {
            if (move != nullptr) {
                // Prune current board state of evals to save memory, then update board
                boardStates.back()->PruneBoard();
                board->MakeMove(move.get());

                // Send move to all players
                for (auto& i : players) {
                    if (i != nullptr) {
                        i->MakeMove(move.get());
                    }
                }

                // Immediately show move if it is a human move
                if (move->IsHumanMove()) {
                    callback(board->GetTreeData(), move->GetCopy());
                }

                // Send
                boardStates.push_back(board->GetBoard(false));

                // Check if game is over
                if (boardStates.back()->GameIsOver()) {
                    return;
                }

                // Get next move
                currentPlayer++;
                if (currentPlayer == players.end()) {
                    currentPlayer = players.begin();
                }
            }

            if (*currentPlayer != nullptr) { // Checks if player is human or AI
                // Send evaluation data to UI
                auto data = (*currentPlayer)->EvalBoard();
                // Get move
                move = (*currentPlayer)->GetBestMove();
                callback(std::move(data), move->GetCopy());
            }
            else { // Next move is human
                move = nullptr;
            }
            
        } while (move != nullptr);
    });
}

void AbstractGame::StepBack()
{
    assert(callback != nullptr);

    if (uncommitedBoardStateId > 0) {
        uncommitedBoardStateId--;
    }

    callback(boardStates.at(uncommitedBoardStateId)->GetTreeData(), nullptr);
}

void AbstractGame::StepForward()
{
    assert(callback != nullptr);

    if (uncommitedBoardStateId < boardStates.size() - 1) {
        uncommitedBoardStateId++;
    }

    callback(boardStates.at(uncommitedBoardStateId)->GetTreeData(), nullptr);
}

void AbstractGame::StepEnd()
{
    assert(callback != nullptr);

    uncommitedBoardStateId = boardStates.size() - 1;
    callback(boardStates.at(uncommitedBoardStateId)->GetTreeData(), nullptr);
}

void AbstractGame::StepStart()
{
    assert(callback != nullptr);

    uncommitedBoardStateId = 0;
    callback(boardStates.at(uncommitedBoardStateId)->GetTreeData(), nullptr);
}

void AbstractGame::SetStep(size_t step)
{
    assert(callback != nullptr);

    if (step < boardStates.size()) {
        uncommitedBoardStateId = step;
    }

    auto data = boardStates.at(uncommitedBoardStateId)->GetTreeData();
    data = nullptr;
    callback(boardStates.at(uncommitedBoardStateId)->GetTreeData(), nullptr);
}

void AbstractGame::StartFromCurrentState()
{
    assert(callback != nullptr);

    for (size_t i = boardStates.size(); i > uncommitedBoardStateId; i--) {
        boardStates.pop_back();
    }

    MakeMove(nullptr);
}

void AbstractGame::EvaluateBoard(std::unique_ptr<AbstractWeights> weights, float time)
{
    auto currentMoves = boardStates.at(uncommitedBoardStateId)->GetMoves();
    auto currentBoard = GetBoard(7, weights.get(), std::move(currentMoves));
    currentBoard->ExpandBoardEval(time);
    callback(currentBoard->GetTreeData(), nullptr);
}

std::unique_ptr<AbstractBoard> AbstractGame::GetCurrentBoardState() const
{
    return boardStates.back()->GetBoard(false);
}



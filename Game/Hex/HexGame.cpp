#include "HexGame.h"

#include "HexBoard.h"
#include "HexAI.h"

HexGame::HexGame(int size, float time, std::unique_ptr<HexWeights> p1AI, std::unique_ptr<HexWeights> p2AI) :
    AbstractGame(std::make_unique<HexBoard>(size, nullptr))
{
    bool p1IsAi = false;
    if (p1AI != nullptr) {
        players.push_back(std::make_unique<HexAI>(time, std::move(p1AI), size));
        p1IsAi = true;
    }
    else {
        players.push_back(nullptr);
    }
    if (p2AI != nullptr) {
        players.push_back(std::make_unique<HexAI>(time, std::move(p2AI), size));
    }
    else {
        players.push_back(nullptr);
    }

    currentPlayer = players.begin();

    if (p1IsAi) { // Start the game
        players.at(0)->EvalBoard();
        auto move = players.at(0)->GetBestMove();
        this->MakeMove(std::move(move));
    }
}

HexGame::~HexGame()
{

}

bool HexGame::NextMoveIsRed() const
{
    auto currentBoard = static_cast<HexBoard*>(GetCurrentBoardState().release());
    return currentBoard->GetNextPlayerToMove() == 0;
}

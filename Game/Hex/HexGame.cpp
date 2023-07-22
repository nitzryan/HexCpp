#include "HexGame.h"

#include "HexBoard.h"
#include "HexAI.h"
#include <assert.h>

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
        this->MakeMove(nullptr);
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

std::unique_ptr<AbstractBoard> HexGame::GetBoard(int s, AbstractWeights* weights, std::vector<std::unique_ptr<AbstractMove>> moves) const
{
    HexWeights* hexWeights = dynamic_cast<HexWeights*> (weights);
    if (hexWeights == nullptr) { // Invalid weights
        assert(false);
    }

    std::vector<HexMove> hexMoves;
    for (auto& i : moves) {
        HexMove* hexMove = dynamic_cast<HexMove*> (i.get());
        if (hexMove == nullptr) { // Input move array invalid
            assert(false);
        }
        hexMoves.push_back(*hexMove);
    }

    return std::make_unique<HexBoard>(s, hexWeights, hexMoves);
}

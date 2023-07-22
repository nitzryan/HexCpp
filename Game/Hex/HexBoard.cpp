#include "HexBoard.h"
#include "HexTreeData.h"
#include <chrono>
#include <QDebug>
#include <assert.h>

HexBoard::HexBoard(int size, HexWeights* weights)
{
    int size2 = size * size;
    this->size = size;
    moves = std::vector<std::unique_ptr<HexMove>>(0);
    tiles = std::vector<Hex::Tile>(size2);
    for (auto& i : tiles) {
        i = Hex::Tile::Empty;
    }
    helper = std::make_unique<HexBoardHelper>(size);
    root = std::make_unique<HexBoardTree>(size, weights, helper.get());
}

HexBoard::HexBoard(const HexBoard* board)
{
    moves.reserve(board->moves.size());
    for (size_t i = 0; i < board->moves.size(); i++) {
        moves.emplace_back(std::make_unique<HexMove>(*board->moves.at(i)));
    }
    tiles = board->tiles;
    root = std::make_unique<HexBoardTree>(board->root);
    this->size = board->size;
    helper = std::make_unique<HexBoardHelper>(size);
}

HexBoard::HexBoard(int size, HexWeights* weights, std::vector<HexMove> moves)
{
    int size2 = size * size;
    this->size = size;
    this->moves.reserve(moves.size());

    tiles = std::vector<Hex::Tile>(size2);
    for (auto& i : tiles) {
        i = Hex::Tile::Empty;
    }
    for (auto& i : moves)
    {
        this->moves.emplace_back(std::make_unique<HexMove>(i));
        if (i.IsRed())
            tiles.at(i.GetTile()) = Hex::Tile::Red;
        else
            tiles.at(i.GetTile()) = Hex::Tile::Blue;
    }
    
    helper = std::make_unique<HexBoardHelper>(size);
    root = HexBoardTree::CreateTree(size, weights, helper.get(), moves);
}

HexBoard::~HexBoard()
{

}

std::unique_ptr<AbstractBoard> HexBoard::GetBoard(bool expanded) const
{
    auto boardCopy = std::make_unique<HexBoard>(this);

    if (!expanded) {
        boardCopy->PruneBoard();
    }

    return boardCopy;
}

void HexBoard::MakeMove(const AbstractMove* move)
{
    // Get Move
    auto p = dynamic_cast<const HexMove*>(move);
    std::unique_ptr<HexMove> hexMove = std::make_unique<HexMove>(p->IsRed(), p->GetTile(), move->IsHumanMove());
    
    // Update board
    if (root != nullptr) {
        root = root->SelectBranch(hexMove->GetTile());
    }

    // Updates moves and tiles
    tiles.at(hexMove->GetTile()) = hexMove->IsRed() ? Hex::Tile::Red : Hex::Tile::Blue;
    moves.push_back(std::move(hexMove));
}

void HexBoard::ExpandBoardEval(float time)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds((int)(time * 1000.f));
    while (std::chrono::high_resolution_clock::now() < end) {
        root->ExpandTree();
    }
    qDebug() << root->GetBoardsEvaulated();
}

void HexBoard::PruneBoard()
{
    root->PruneTree();
}

void HexBoard::Clear()
{
    root->RemoveChildren();
}

std::unique_ptr<AbstractMove> HexBoard::GetBestMove() const
{
    return std::make_unique<HexMove>(root->GetIsRed(), root->GetBestMove(), false);
}

bool HexBoard::GameIsOver() const
{
    return root->GameIsOver();
}

short HexBoard::GetNextPlayerToMove() const
{
    if (root->GetIsRed()) {
        return 0;
    }
    return 1;
}

bool HexBoard::IsMoveLegal(const std::unique_ptr<AbstractMove> &move) const
{
    auto p = dynamic_cast<HexMove*>(move.get());
    return root->isMoveLegal(p->GetTile());
}

std::vector<std::string> HexBoard::GetMoveNotations() const
{
    auto result = std::vector<std::string>(moves.size());

    for (size_t i = 0; i < moves.size(); i++) {
        result.at(i) = moves.at(i)->GetNotation();
    }

    return result;
}

std::vector<Hex::Tile> HexBoard::GetTiles() const
{
    return tiles;
}

int HexBoard::GetSize() const
{
    return size;
}

std::unique_ptr<AbstractTreeData> HexBoard::GetTreeData() const
{
    return std::make_unique<HexTreeData>(tiles, moves, root->GetNode(nullptr));
}

std::unique_ptr<AbstractMove> HexBoard::GetChosenMove() const
{
    std::unique_ptr<HexMove> hexMove;
    root->SetTile(hexMove->GetTile());
    return std::move(hexMove);
}

void HexBoard::SetWeights(AbstractWeights* weights)
{
    root->SetWeights(dynamic_cast<HexWeights*>(weights));
}

std::vector<std::unique_ptr<AbstractMove>> HexBoard::GetMoves() const
{
    std::vector<std::unique_ptr<AbstractMove>> returnMoves;
    returnMoves.reserve(moves.size());
    for (auto& i : moves) {
        returnMoves.push_back(i->GetCopy());
    }
    return returnMoves;
}


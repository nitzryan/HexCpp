#include "AbstractAI.h"
#include <assert.h>

std::unique_ptr<AbstractMove> AbstractAI::GetBestMove() const
{
    auto move = board->GetBestMove();
    return std::move(move);
}

std::unique_ptr<AbstractTreeData> AbstractAI::EvalBoard() const
{
    board->ExpandBoardEval(time);
    return board->GetTreeData();
}

void AbstractAI::MakeMove(AbstractMove* move)
{
    board->MakeMove(move);
}
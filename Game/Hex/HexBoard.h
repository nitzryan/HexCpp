#ifndef HEXBOARD_H
#define HEXBOARD_H

#include "../AbstractBoard.h"
#include "HexMove.h"
#include "HexBoardTree.h"
#include "HexBoardHelper.h"
#include <vector>
#include <memory>

class HexBoard : public AbstractBoard
{
public:
    /**
     * @brief HexBoard
     */
    HexBoard(int size, HexWeights* weights);
    /**
     * @brief HexBoard
     * @param board
     */
    HexBoard(const HexBoard* board);
    /**
     * @brief ~HexBoard
     */
    ~HexBoard();
    /**
     * @brief GetBoard
     * @param expanded
     * @return
     */
    std::unique_ptr<AbstractBoard> GetBoard(bool expanded) const override;
    /**
     * @brief MakeMove
     * @param move
     */
    void MakeMove(const AbstractMove* move) override;
    /**
     * @brief ExpandBoardEval
     * @param time
     * @param weights
     */
    void ExpandBoardEval(float time) override;
    /**
     * @brief PruneBoard
     */
    void PruneBoard() override;
    /**
     * @brief GetBestMove
     * @return
     */
    std::unique_ptr<AbstractMove> GetBestMove() const override;
    /**
     * @brief GameIsOver
     * @return
     */
    bool GameIsOver() const override;
    /**
     * @brief GetNextPlayerToMove
     * @return
     */
    short GetNextPlayerToMove() const override;
    /**
     * @brief IsMoveLegal
     * @param move
     * @return
     */
    bool IsMoveLegal(const std::unique_ptr<AbstractMove>& move) const override;
    std::vector<std::string> GetMoveNotations() const;
    std::vector<Hex::Tile> GetTiles() const;
    int GetSize() const;
    std::unique_ptr<AbstractTreeData> GetTreeData() const override;
    std::unique_ptr<AbstractMove> GetChosenMove() const override;
private:
    int size;
    std::vector<std::unique_ptr<HexMove>> moves;
    std::vector<Hex::Tile> tiles;
    std::unique_ptr<HexBoardTree> root;
    std::unique_ptr<HexBoardHelper> helper;
};

#endif // HEXBOARD_H

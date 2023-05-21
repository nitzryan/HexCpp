#ifndef HEXBOARDTREE_H
#define HEXBOARDTREE_H

#include <vector>
#include "HexWeights.h"
#include "HexTreeNode.h"
#include "HexMove.h"
#include "Chain.h"
#include "Template.h"
#include "HexBoardHelper.h"
#include "Edge.h"

class HexBoardTree
{
public:
    /**
     * @brief HexBoardTree
     * @param numTiles
     */
    HexBoardTree(short numTiles, HexWeights* weights, HexBoardHelper* h, std::vector<HexMove> moveList = {});
    /**
     * @brief HexBoardTree
     * @param base
     */
    HexBoardTree(const std::unique_ptr<HexBoardTree>& base);
    /**
     * @brief HexBoardTree
     * @param base
     * @param move
     */
    HexBoardTree(const HexBoardTree& base, short move);
    ~HexBoardTree();
    /**
     * @brief ExpandTree
     */
    void ExpandTree();
    /**
     * @brief GetBestMove
     * @return
     */
    short GetBestMove();
    /**
     * @brief PruneTree
     */
    void PruneTree();
    /**
     * @brief SelectBranch
     * @param move
     */
    std::unique_ptr<HexBoardTree> SelectBranch(short move);
    /**
     * @brief GetIsRed
     * @return
     */
    bool GetIsRed() const;
    /**
     * @brief isMoveLegal
     * @return
     */
    bool isMoveLegal(short) const;
    /**
     * @brief GetNode
     * @return
     */
    std::shared_ptr<HexTreeNode> GetNode(std::shared_ptr<HexTreeNode> parent) const;
    bool GameIsOver() const;
    void SetTile(short t);

    const std::vector<Chain>* GetChainList(bool red) const;
private:
    std::vector<std::unique_ptr<HexBoardTree>> children;
    std::vector<short> remainingMoves;
    bool isRed;
    bool fullyTraversed = false;
    short size;
    short move;
    short boardTiles;
    float heuristicScore;
    float score;
    float searchScoreRed, searchScoreBlue;
    HexWeights weights;
    float centerScore;

    std::vector<Chain> redChains, blueChains;
    std::vector<Template> redTemplates, blueTemplates;
    std::vector<Edge> redEdges, blueEdges;
    HexBoardHelper* helper;
};

#endif // HEXBOARDTREE_H

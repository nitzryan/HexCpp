#include "HexBoardTree.h"
#include <limits>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include "HBTMethods.h"

#define MAX_SCORE 10000.0f

HexBoardTree::HexBoardTree(short s, HexWeights* w, HexBoardHelper* h)
{
    short numTiles = s * s;
    children.reserve(numTiles);
    remainingMoves = std::vector<short>(numTiles);
    for (std::size_t i = 0; i < remainingMoves.size(); i++) {
        remainingMoves[i] = static_cast<short>(i);
    }
    isRed = true;
    assert(w != nullptr);
    weights = w;
    move = -1;
    boardTiles = numTiles;
    size = s;
    helper = h;
    // Scores
    centerScore = s * w->DepthPenalty * 0.5f;
    heuristicScore = 0;
    searchScoreRed = 0;
    searchScoreBlue = 0;
    score = 0;

    // BitArrays
    placedTiles = BitArray(boardTiles);
    redTiles = BitArray(boardTiles);
    blueTiles = BitArray(boardTiles);

    redPoison = Poison(s, true);
    bluePoison = Poison(s, false);
}

HexBoardTree::HexBoardTree(const std::unique_ptr<HexBoardTree>& base)
{
    // Create Deep Copy
    children.reserve(base->remainingMoves.size());
    remainingMoves = base->remainingMoves;
    isRed = base->isRed;
    weights = base->weights;
    move = base->move;
    boardTiles = base->boardTiles;
    size = base->size;
    helper = base->helper;
    for (auto& i : base->children) {
        children.push_back(std::make_unique<HexBoardTree>(i));
    }

    // Scores
    heuristicScore = base->heuristicScore;
    score = base->score;
    searchScoreBlue = base->searchScoreBlue;
    searchScoreRed = base->searchScoreRed;
    centerScore = base->centerScore;

    // BitArrays
    placedTiles = BitArray(&base->placedTiles);
    redTiles = BitArray(&base->redTiles);
    blueTiles = BitArray(&base->blueTiles);

    // Chains/Templates/Edges (needs mapping)
    redChains = base->redChains;
    blueChains = base->blueChains;
    redTemplates = base->redTemplates;
    blueTemplates = base->blueTemplates;
    redEdges = base->redEdges;
    blueEdges = base->blueEdges;
    rsEdges = base->rsEdges;
    bsEdges = base->bsEdges;

    redPoison = Poison(base->redPoison);
    bluePoison = Poison(base->bluePoison);
}

HexBoardTree::HexBoardTree(const HexBoardTree& base, short mv)
{
    CopyPreviousState(base, mv);
    
    // Point to lists based on same/opposite color
    auto templateList = base.isRed ? &redTemplates : &blueTemplates;
    auto otherTemplateList = base.isRed ? &blueTemplates : &redTemplates;
    auto sEdgeList = base.isRed ? &rsEdges : &bsEdges;
    auto sOthEdgeList = base.isRed ? &bsEdges : &rsEdges;
    auto edgeList = base.isRed ? &redEdges : &blueEdges;
    auto otherEdgeList = base.isRed ? &blueEdges : &redEdges;
    auto othPoison = base.isRed ? &bluePoison : &redPoison;
    auto chainList = base.isRed ? &redChains : &blueChains;
    auto otherChainList = base.isRed ? &blueChains : &redChains;
    auto oldList = base.isRed ? &base.redChains : &base.blueChains;
    auto thisTiles = base.isRed ? &redTiles : &blueTiles;
    auto oppTiles = base.isRed ? &blueTiles : &redTiles;

    
    
    HBTMethods::CreateChain(mv, size, base.isRed, *chainList, *oldList, *templateList, *edgeList, *sEdgeList, placedTiles, *thisTiles, *oppTiles, helper);
    if (base.isRed)
        HBTMethods::RemapAddresses(blueTemplates, blueEdges, bsEdges, base.blueChains, blueChains);
    else
        HBTMethods::RemapAddresses(redTemplates, redEdges, rsEdges, base.redChains, redChains);

    othPoison->PlaceTile(mv, size, base.isRed);

    HBTMethods::PoisonUpdateTilesRanks(redPoison, size, false, helper, redChains, blueTemplates, blueEdges, bsEdges);
    HBTMethods::PoisonUpdateTilesRanks(bluePoison, size, true, helper, blueChains, redTemplates, redEdges, rsEdges);

    auto longRed = HBTMethods::GetLongestChainLength(redChains);
    auto longBlue = HBTMethods::GetLongestChainLength(blueChains);
    auto longTemplateRed = HBTMethods::GetLongestTemplateLength(redChains, redTemplates, redEdges, rsEdges);
    auto longTemplateBlue = HBTMethods::GetLongestTemplateLength(blueChains, blueTemplates, blueEdges, bsEdges);

    centerScore = base.centerScore;
    heuristicScore = HBTMethods::GetScore(longRed, longBlue, longTemplateRed, longTemplateBlue, centerScore, mv, size, base.isRed, redTiles, blueTiles, weights);
    searchScoreBlue = heuristicScore;
    searchScoreRed = heuristicScore;
    score = heuristicScore;

    if (std::isnan(score) || std::abs(score) > 2 * MAX_SCORE) { // Should not occur
        score = score + 1;
    }

    HBTMethods::ApplyMoveToOpponentStructures(mv, *otherChainList, *otherTemplateList, *sOthEdgeList);
    HBTMethods::RemoveBrokenTemplates(*otherTemplateList, *otherEdgeList, *sOthEdgeList, helper);
}

std::unique_ptr<HexBoardTree> HexBoardTree::CreateTree(short numTiles, HexWeights* w, HexBoardHelper* h, std::vector<HexMove> moveList)
{
    std::unique_ptr<HexBoardTree> tree = std::make_unique<HexBoardTree>(numTiles, w, h);
    for (auto& i : moveList) {
        tree = tree->SelectBranch(i.GetTile());
    }

    return tree;
}

HexBoardTree::~HexBoardTree()
{

}

void HexBoardTree::ExpandTree()
{
    if (children.size() == 0) { // At the edge of the tree, need to create children
        for (size_t i = 0; i < remainingMoves.size(); i++) {
            children.push_back(std::make_unique<HexBoardTree>(*this, remainingMoves[i]));
        }
    } else { // Nodes created, call on most promising path
        // Run Recursively
        int best = -1;
        bool solutionFound = true; // Should be set, but if not don't explore further as something went wrong
        if (isRed) {
            auto maxRed = -std::numeric_limits<float>::infinity();

            for (size_t i = 0; i < children.size(); i++) {
                if (children.at(i)->searchScoreRed > maxRed) {
                    maxRed = children.at(i)->searchScoreRed;
                    solutionFound = children.at(i)->fullyTraversed;
                    best = static_cast<int>(i);
                }
            }
        } else {
            auto maxBlue = std::numeric_limits<float>::infinity();
            for (size_t i = 0; i < children.size(); i++) {
                if (children.at(i)->searchScoreBlue < maxBlue) {
                    maxBlue = children.at(i)->searchScoreBlue;
                    solutionFound = children.at(i)->fullyTraversed;
                    best = static_cast<int>(i);
                }
            }
        }
        if (best == -1 || solutionFound) {
            fullyTraversed = true;
            return;
        }
        children.at(best)->ExpandTree();
    }

    // Update Scores
    if (isRed) {
        score = -std::numeric_limits<float>::infinity();
        searchScoreRed = score;
        searchScoreBlue = score;
        for (auto& i : children) {
            if (i->score > score) {
                score = i->score;
            }
            auto redSearch = i->searchScoreRed - weights->DepthPenalty;
            if (redSearch > searchScoreRed) {
                searchScoreRed = redSearch;
            }
            auto blueSearch = i->searchScoreBlue + weights->DepthPenalty;
            if (blueSearch > searchScoreBlue) {
                searchScoreBlue = blueSearch;
            }
        }
    } else {
        score = std::numeric_limits<float>::infinity();
        searchScoreRed = score;
        searchScoreBlue = score;
        for (auto& i : children) {
            if (i->score < score) {
                score = i->score;
            }
            auto redSearch = i->searchScoreRed - weights->DepthPenalty;
            if (redSearch < searchScoreRed) {
                searchScoreRed = redSearch;
            }
            auto blueSearch = i->searchScoreBlue + weights->DepthPenalty;
            if (blueSearch < searchScoreBlue) {
                searchScoreBlue = blueSearch;
            }
        }
    }

}

short HexBoardTree::GetBestMove()
{
    for (std::size_t i = 0; i < children.size(); i++) {
        if (children[i]->score == score) {
            return remainingMoves[i];
        }
    }

    // This should not happen, means no child had same score as this node, meaning there is a bug
    assert(false);
    return remainingMoves[0];
}

void HexBoardTree::PruneTree()
{
    for (auto& i : children) {
        if (i != nullptr) {
            i->children.clear();
        }
    }
}

void HexBoardTree::RemoveChildren()
{
    children.clear();
    fullyTraversed = false;
}

std::unique_ptr<HexBoardTree> HexBoardTree::SelectBranch(short m)
{
    // Find move index in vectors
    std::size_t index = std::numeric_limits<std::size_t>::max();
    for (std::size_t i = 0; i < remainingMoves.size(); i++) {
        if (remainingMoves[i] == m) {
            index = i;
            break;
        }
    }

    // Remove from children, so that when (this) is deleted, that child will not be deleted
    std::unique_ptr<HexBoardTree> node = children.empty() ? nullptr : std::move(children.at(index));

    if (node == nullptr) {
        node = std::make_unique<HexBoardTree>(*this, m);
    }

    return node;
}

bool HexBoardTree::GetIsRed() const
{
    return isRed;
}

bool HexBoardTree::isMoveLegal(short m) const
{
    for (auto i : remainingMoves) {
        if (i == m) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<HexTreeNode> HexBoardTree::GetNode(std::shared_ptr<HexTreeNode> parent) const
{
    std::shared_ptr<HexTreeNode> node = std::make_shared<HexTreeNode>(score,
                                                                      heuristicScore,
                                                                      move,
                                                                      isRed ? Hex::Tile::Red : Hex::Tile::Blue,
                                                                      parent);
    for (auto& i : children) {
        node->AddChild(i->GetNode(node));
    }
    return node;
}

bool HexBoardTree::GameIsOver() const
{
    for (auto& i : redChains) {
        if (i.GetChainLength() == size - 1) {
            return true;
        }
    }

    for (auto& i : blueChains) {
        if (i.GetChainLength() == size - 1) {
            return true;
        }
    }

    return false;
}

void HexBoardTree::SetTile(short t)
{
    move = t;
}

const std::vector<Chain>* HexBoardTree::GetChainList(bool red) const
{
    if (red) {
        return &redChains;
    }
    return &blueChains;
}

long HexBoardTree::GetBoardsEvaulated() const
{
    long count = 0;
    for (auto& i : children)
        count += i->GetBoardsEvaulated();
    return count + 1;
}

void HexBoardTree::CopyPreviousState(const HexBoardTree& base, short mv)
{
    children.reserve(base.remainingMoves.size());
    remainingMoves = base.remainingMoves;
    remainingMoves.erase(std::find(remainingMoves.begin(), remainingMoves.end(), mv)); // No error check, it will exist

    isRed = !base.isRed;
    weights = base.weights;
    this->move = mv;
    boardTiles = base.boardTiles;
    size = base.size;
    helper = base.helper;

    // BitArrays
    placedTiles = BitArray(&base.placedTiles);
    placedTiles.SetBit(move);
    redTiles = BitArray(&base.redTiles);
    blueTiles = BitArray(&base.blueTiles);

    // Copy chains, reserve space for new chain
    redChains = base.redChains;
    redChains.reserve(redChains.size() + 1);
    blueChains = base.blueChains;
    blueChains.reserve(blueChains.size() + 1);
    redTemplates = base.redTemplates;
    redTemplates.reserve(redTemplates.size() + 1);
    blueTemplates = base.blueTemplates;
    blueTemplates.reserve(blueTemplates.size() + 1);
    redEdges = base.redEdges;
    redEdges.reserve(redEdges.size() + 1);
    blueEdges = base.blueEdges;
    blueEdges.reserve(blueEdges.size() + 1);
    rsEdges = base.rsEdges;
    rsEdges.reserve(rsEdges.size() + 1);
    bsEdges = base.bsEdges;
    bsEdges.reserve(bsEdges.size() + 1);

    redPoison = Poison(base.redPoison);
    bluePoison = Poison(base.bluePoison);
}

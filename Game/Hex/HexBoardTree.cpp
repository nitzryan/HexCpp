#include "HexBoardTree.h"
#include <limits>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <functional>

#define MAX_SCORE 10000.0f

HexBoardTree::HexBoardTree(short s, HexWeights* w, HexBoardHelper* h, std::vector<HexMove> moveList)
{
    short numTiles = s * s;
    children.reserve(numTiles);
    remainingMoves = std::vector<short>(numTiles);
    for (std::size_t i = 0; i < remainingMoves.size(); i++) {
        remainingMoves[i] = static_cast<short>(i);
    }
    isRed = true;
    weights = (w == nullptr) ? HexWeights{} : *w;
    move = -1;
    boardTiles = numTiles;
    size = s;
    helper = h;
    // Empty board, so no scores necessary except center score bias
    centerScore = s * weights.DepthPenalty * 0.5f;

    /* Make moves in moveList to get Tree State
        Necessary since some algorithms used to get score require knowledge
        From previous stages, so to get score, need to build tree move by move
    */
    for (auto& i : moveList) {
        ExpandTree();
        SelectBranch(i.GetTile());
    }
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

    // Chains/Templates/Edges
    redChains = base->redChains;
    blueChains = base->blueChains;
    redTemplates = base->redTemplates;
    blueTemplates = base->blueTemplates;
    redEdges = base->redEdges;
    blueEdges = base->blueEdges;
}

HexBoardTree::HexBoardTree(const HexBoardTree& base, short move)
{
    // Initialize
    children.reserve(base.remainingMoves.size());
    remainingMoves = base.remainingMoves;
    remainingMoves.erase(std::find(remainingMoves.begin(), remainingMoves.end(), move)); // No error check, it will exist

    isRed = !base.isRed;
    weights = base.weights;
    this->move = move;
    boardTiles = base.boardTiles;
    size = base.size;
    helper = base.helper;

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

    // Remove templates of current color that were broken by opponent
    auto templateList = base.isRed ? &redTemplates : &blueTemplates;
    auto otherTemplateList = base.isRed ? &blueTemplates : &redTemplates;
    templateList->erase(std::remove_if(templateList->begin(), templateList->end(), [](const Template& t) {
        return t.IsBroken();
        }), templateList->end());

    // Set all chains/templates as non-traversed
    for (auto& i : redTemplates) {
        i.SetVisited(false);
    }
    for (auto& i : blueTemplates) {
        i.SetVisited(false);
    }
    for (auto& i : redChains) {
        i.SetTraversed(false);
    }
    for (auto& i : blueChains) {
        i.SetTraversed(false);
    }

    // Create new Chain
    auto chainList = base.isRed ? &redChains : &blueChains;
    auto oldList = base.isRed ? &base.redChains : &base.blueChains;
    Chain newChain(move, size, base.isRed);
    for (auto& i : *chainList) {
        if (newChain.ShouldMerge(i)) {
            newChain.MergeWith(&i);
        }
    }

    // Cleanup any chains that were merged with new chain
    // Keep track of deleted chains to use in address remapping
    std::vector<bool> didDelete(chainList->size());
    for (size_t i = 0; i < chainList->size(); i++) {
        didDelete[i] = chainList->at(i).ShouldDelete();
    }
    chainList->erase(std::remove_if(chainList->begin(), chainList->end(), [](const Chain& c) {
        return c.ShouldDelete();
        }), chainList->end());
    

    // Delete any Template that has a Chain that was deleted
    for (size_t i = 0; i < oldList->size(); i++) {
        if (didDelete[i]) {
            for (int t = (int)templateList->size() - 1; t >= 0; t--) {
                if (templateList->at(t).ContainsChain(&oldList->at(i))) {
                    templateList->erase(templateList->begin() + t);
                }
            }
        }
    }

    // Remap addresses of same color templates
    size_t jj = 0;
    for (size_t i = 0; i < oldList->size(); i++) {
        if (!didDelete[i]) {
            for (auto& t : *templateList) {
                t.Remap(&(*oldList)[i], &(*chainList)[jj]);
            }
            jj++;
        }
    }

    // Remap addresses of other color templates
    auto otherChainList = base.isRed ? &blueChains : &redChains;
    auto otherOldList = base.isRed ? &base.blueChains : &base.redChains;
    for (size_t i = 0; i < otherOldList->size(); i++) {
        for (auto& t : *otherTemplateList) {
            auto c1 = &(*otherOldList)[i];
            auto c2 = &(*otherChainList)[i];
            t.Remap(c1, c2);
        }
    }

    // Check other chains for templates with new chain
    chainList->push_back(newChain);
    
    for (size_t i = 0; i < chainList->size() - 1; i++) {
        if (Template::IsATemplate(&chainList->back(), &(*chainList)[i])) {
            templateList->push_back(Template(&chainList->back(), &chainList->at(i)));
        }
    }

    // Check that all mappings are valid
    for (auto& t : redTemplates) {
        bool found = false;
        for (auto& j : redChains) {
            for (auto& i : redChains) {
                const Chain* a1, * a2;
                t.GetChains(&a1, &a2);
                if (&i != &j && ((&i == a1 && &j == a2) || (&i == a2 && &j == a1))) {
                    found = true;
                }
            }
        }
        if (found == false) {
            break;
        }
    }
    for (auto& t : blueTemplates) {
        bool found = false;
        for (auto& j : blueChains) {
            for (auto& i : blueChains) {
                const Chain* a1, * a2;
                t.GetChains(&a1, &a2);
                if (&i != &j && ((&i == a1 && &j == a2) || (&i == a2 && &j == a1))) {
                    found = true;
                }
            }
        }
        if (found == false) {
            break;
        }
    }

    // Apply move to oppenent Chain/Edge/Template Adjacencies
    for (auto& i : blueChains) {
        i.OpponentTilePlaced(move);
    }
    for (auto& i : blueTemplates) {
        i.CheckToBreak(move);
    }

    // Calculate Heuristic Score
    // Center Penalty
    auto halfSize = size / 2;
    auto row = move / size;
    auto col = move % size;
    int rowPlusColOffset = (row < halfSize && col > halfSize) || (row > halfSize && col < halfSize) ?
        std::max(std::abs(row - halfSize), std::abs(col - halfSize)) :
        std::abs(row - halfSize) + std::abs(col - halfSize);
    centerScore = base.centerScore;
    if (base.isRed) {
        centerScore -= rowPlusColOffset * weights.CenterPenalty;
    } else {
        centerScore += rowPlusColOffset * weights.CenterPenalty;
    }
    heuristicScore = centerScore;

    // Longest chain for red/blue
    auto longestChainLength = [](std::vector<Chain>& c) {
        int longest = 0;
        for (auto& i : c) {
            auto l = i.GetChainLength();
            if (l > longest) {
                longest = l;
            }
        }
        return longest;
    };

    auto longRed = longestChainLength(redChains);
    auto longBlue = longestChainLength(blueChains);

    // Longest Template/Edge Length
    auto longestTemplateLength = [](std::vector<Chain>& c, std::vector<Template>& t) {
        // Checks all templates, and gets min/max rank of other chains on that template
        std::function<void(int&, int&, Chain*, std::vector<Template>&)> getRank = 
                            [&](int& min, int& max, Chain* chain, std::vector<Template>& templates) {
            if (chain->Traversed()) {
                return;
            }
            // Update Min/Max to include this chain
            auto minr = chain->GetMinRank();
            auto maxr = chain->GetMaxRank();
            min = (minr < min) ? minr : min;
            max = (maxr > max) ? maxr : max;
            chain->SetTraversed(true);
            // Recursively traverse templates
            for (auto& t : templates) {
                Chain* otherChain = t.GetOtherChain(chain);
                if (otherChain != nullptr && !otherChain->Traversed()) {
                    getRank(min, max, otherChain, templates);
                }
            }
        };

        // Iterate through chains to find longest template
        int longest = 0;
        for (auto& i : c) {
            int minRank = std::numeric_limits<int>::max();
            int maxRank = -1;
            getRank(minRank, maxRank, &i, t);
            auto rankDif = maxRank - minRank;
            longest = (rankDif > longest) ? rankDif : longest;
        }
        return longest;
    };

    auto longTemplateRed = longestTemplateLength(redChains, redTemplates);
    auto longTemplateBlue = longestTemplateLength(blueChains, blueTemplates);

    // Appply lengths to scores
    if (longRed == size - 1) { // Red Wins
        heuristicScore = MAX_SCORE + remainingMoves.size();
        fullyTraversed = true;
    } else if (longBlue == size - 1) { // Blue Wins
        heuristicScore = -(MAX_SCORE + remainingMoves.size());
        fullyTraversed = true;
    } else { // Neither player has won
        heuristicScore += longRed * weights.ChainLengthFactor;
        heuristicScore += longTemplateRed * weights.TemplateLengthFactor;
        heuristicScore -= longBlue * weights.ChainLengthFactor;
        heuristicScore -= longTemplateBlue * weights.TemplateLengthFactor;
    }

    // Set Scores from Heuristic Score

    score = heuristicScore;
    searchScoreBlue = heuristicScore;
    searchScoreRed = heuristicScore;

    if (std::isnan(score) || std::abs(score) > 2 * MAX_SCORE) { // Should not occur
        score = score + 1;
    }
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
            auto redSearch = i->searchScoreRed - weights.DepthPenalty;
            if (redSearch > searchScoreRed) {
                searchScoreRed = redSearch;
            }
            auto blueSearch = i->searchScoreBlue + weights.DepthPenalty;
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
            auto redSearch = i->searchScoreRed - weights.DepthPenalty;
            if (redSearch < searchScoreRed) {
                searchScoreRed = redSearch;
            }
            auto blueSearch = i->searchScoreBlue + weights.DepthPenalty;
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

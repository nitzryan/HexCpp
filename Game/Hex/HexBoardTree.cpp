#include "HexBoardTree.h"
#include <limits>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <functional>

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
    weights = (w == nullptr) ? HexWeights{} : *w;
    move = -1;
    boardTiles = numTiles;
    size = s;
    helper = h;
    // Scores
    centerScore = s * weights.DepthPenalty * 0.5f;
    heuristicScore = 0;
    searchScoreRed = 0;
    searchScoreBlue = 0;
    score = 0;

    // BitArrays
    placedTiles = BitArray(boardTiles);
    redAdjacencies = BitArray(boardTiles);
    blueAdjacencies = BitArray(boardTiles);
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
    redAdjacencies = BitArray(&base->redAdjacencies);
    blueAdjacencies = BitArray(&base->blueAdjacencies);

    // Chains/Templates/Edges (needs mapping)
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

    // BitArrays
    placedTiles = BitArray(&base.placedTiles);
    placedTiles.SetBit(move);
    redAdjacencies = BitArray(&base.redAdjacencies);
    redAdjacencies.UnsetBit(move);
    blueAdjacencies = BitArray(&base.blueAdjacencies);
    blueAdjacencies.UnsetBit(move);

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

    
    auto edgeList = base.isRed ? &redEdges : &blueEdges;
    auto otherEdgeList = base.isRed ? &blueEdges : &redEdges;
    

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
    Chain newChain(move, size, base.isRed, &placedTiles);
    for (auto& i : *chainList) {
        if (newChain.ShouldMerge(i)) {
            newChain.MergeWith(&i);
        }
    }

    // Update adjacencies
    if (isRed) {
        redAdjacencies = BitArray::Or(&redAdjacencies, newChain.GetAdjacencies());
    }
    else {
        blueAdjacencies = BitArray::Or(&blueAdjacencies, newChain.GetAdjacencies());
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
    

    // Delete any Template or Edge that has a Chain that was deleted
    for (size_t i = 0; i < oldList->size(); i++) {
        if (didDelete[i]) {
            for (int t = (int)templateList->size() - 1; t >= 0; t--) {
                if (templateList->at(t).ContainsChain(&oldList->at(i))) {
                    templateList->erase(templateList->begin() + t);
                }
            }
            for (int e = (int)edgeList->size() - 1; e >= 0; e--) {
                if (&oldList->at(i) == edgeList->at(e).GetChain()){
                    edgeList->erase(edgeList->begin() + e);
                }
            }
        }
    }

    // Remap addresses of same color templates/edges
    size_t jj = 0;
    for (size_t i = 0; i < oldList->size(); i++) {
        if (!didDelete[i]) {
            for (auto& t : *templateList) {
                t.Remap(&(*oldList)[i], &(*chainList)[jj]);
            }
            for (auto& e : *edgeList) {
                e.RemapAddresses(&(*oldList)[i], &(*chainList)[jj]);
            }
            jj++;
        }
    }

    // Remap addresses of other color templates
    auto otherChainList = base.isRed ? &blueChains : &redChains;
    auto otherOldList = base.isRed ? &base.blueChains : &base.redChains;
    for (size_t i = 0; i < otherOldList->size(); i++) {
        for (auto& t : *otherTemplateList) {
            t.Remap(&(*otherOldList)[i], &(*otherChainList)[i]);
        }
        for (auto& e : *otherEdgeList) {
            e.RemapAddresses(&(*otherOldList)[i], &(*otherChainList)[i]);
        }
    }

    // Remove edges of current color which were broken by opponent last move
    edgeList->erase(std::remove_if(edgeList->begin(), edgeList->end(), [this](const Edge& e) {
        return e.IsStillEdge(helper);
        }), edgeList->end());

    // Check other chains for templates with new chain
    chainList->push_back(newChain);
    
    for (size_t i = 0; i < chainList->size() - 1; i++) {
        if (Template::IsATemplate(&chainList->back(), &(*chainList)[i])) {
            templateList->push_back(Template(&chainList->back(), &chainList->at(i)));
        }
    }

    // Check if new chain should be an edge
    if (Edge::ShouldBeEdge(&chainList->back(), 0, helper)) {
        edgeList->push_back(Edge(&chainList->back(), 0, chainList->back().IsRed()));
    }
    if (Edge::ShouldBeEdge(&chainList->back(), (char)(size - 1), helper)) {
        edgeList->push_back(Edge(&chainList->back(), (char)(size - 1), chainList->back().IsRed()));
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
    for (auto& i : *otherChainList) {
        i.OpponentTilePlaced(move);
    }
    for (auto& i : *otherTemplateList) {
        i.CheckToBreak(move);
    }

    if (!newChain.IsRed()) {
        heuristicScore = heuristicScore;
    }
    if (newChain.IsRed()) {
        heuristicScore = heuristicScore;
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
    auto longestTemplateLength = [](std::vector<Chain>& c, std::vector<Template>& t, std::vector<Edge>& e) {
        // Checks all templates, and gets min/max rank of other chains on that template
        std::function<void(int&, int&, Chain*, std::vector<Template>&, std::vector<Edge>&)> getRank = 
                            [&](int& min, int& max, Chain* chain, std::vector<Template>& templates, std::vector<Edge>& edges) {
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
                    getRank(min, max, otherChain, templates, edges);
                }
            }
            // Check if edge increases or decreases rank
            for (auto& e : edges) {
                if (e.GetChain() == chain) {
                    auto rank = e.GetRank();
                    if (rank < min) {
                        min = 0;
                    }
                    else {
                        max = rank;
                    }
                }
            }
        };

        // Iterate through chains to find longest template
        int longest = 0;
        for (auto& i : c) {
            int minRank = std::numeric_limits<int>::max();
            int maxRank = -1;
            getRank(minRank, maxRank, &i, t, e);
            auto rankDif = maxRank - minRank;
            longest = (rankDif > longest) ? rankDif : longest;
        }
        return longest;
    };

    auto longTemplateRed = longestTemplateLength(redChains, redTemplates, redEdges);
    auto longTemplateBlue = longestTemplateLength(blueChains, blueTemplates, blueEdges);

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

    // Add bias to prevent score oscillation
    if (!isRed) {
        heuristicScore -= weights.TempoBias;
    }

    // Set Scores from Heuristic Score

    score = heuristicScore;
    searchScoreBlue = heuristicScore;
    searchScoreRed = heuristicScore;

    if (std::isnan(score) || std::abs(score) > 2 * MAX_SCORE) { // Should not occur
        score = score + 1;
    }
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

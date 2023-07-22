#include "HexController.h"
#include <QDebug>

HexController::HexController(QObject* parent) : QObject(parent)
{
    hexWeights = nullptr;
}

HexController::~HexController()
{
    if (hexWeights != nullptr) {
        delete hexWeights;
    }
}

void HexController::BoardstateCallback(std::unique_ptr<AbstractTreeData> newBoardTree, std::unique_ptr<AbstractMove> move)
{
    lastMove = std::unique_ptr<HexMove>(static_cast<HexMove*>(move.release()));
    tree.reset(dynamic_cast<HexTreeData*>(newBoardTree.release()));
    auto moves = tree->GetMoves();

    std::vector<std::string> moveNotations;
    moveNotations.reserve(moves.size() + 1);
    for (auto& i : moves) {
        moveNotations.emplace_back(i->GetNotation());
    }

    if (lastMove != nullptr) {
        moveNotations.emplace_back(lastMove->GetNotation());
        emit BoardMoves(moveNotations);
    }
    
    EmitTreeSignals(tree->GetRoot());
}

void HexController::NewGameRequested(int size, float time, bool P1AI, bool P2AI)
{
    // Create game
    std::unique_ptr<HexWeights> p1Weights = P1AI ? std::make_unique<HexWeights>(size) : nullptr;
    std::unique_ptr<HexWeights> p2Weights = P2AI ? std::make_unique<HexWeights>(size) : nullptr;
    if (hexWeights != nullptr)
        delete hexWeights;
    hexWeights = new HexWeights(size);
    game.reset(new HexGame(size, time, std::move(p1Weights), std::move(p2Weights), hexWeights));

    // Setup callback for UI
    auto callbackPtr = [this](std::unique_ptr<AbstractTreeData> b, std::unique_ptr<AbstractMove> m) {
        this->BoardstateCallback(std::move(b), std::move(m));
    };
    game->AddBoardstateCallback(callbackPtr);

    // Signal to UIBoard to make new board
    std::vector<UI::Tile> tiles;
    tiles.reserve(size * size);
    for (size_t i = 0; i < size * size; i++) {
        tiles.push_back(UI::Tile{ std::to_string(i), UI::TileColor::Empty });
    }
    emit BoardState(tiles);
    emit BoardMoves({});
}

void HexController::MoveSelected(int move)
{
    game->SetStep((size_t)move);
}

void HexController::EvaluatePosition(float time)
{
    game->EvaluateBoard(std::make_unique<HexWeights>(game->GetSize()), time);
}

void HexController::CommitPosition()
{
    game->StartFromCurrentState();
}

void HexController::TileClicked(int tile)
{
    game->MakeMove(std::make_unique<HexMove>(game->NextMoveIsRed(), tile, true));
}

void HexController::EmitTreeSignals(std::shared_ptr<HexTreeNode> node)
{
    // Create Tiles
    auto hexTiles = tree->GetTiles();
    auto size = hexTiles.size();
    std::vector<UI::Tile> tiles(size);
    for (auto& i : tiles) {
        i.text = "";
    }

    // Set text for tiles not placed
    for (auto& i : node->GetChildren()) {
        short tile = i->GetTile();
        tiles.at(tile).text = std::to_string(i->GetScore());
    }

    // Set color for tiles already placed and tiles placed in eval tree
    auto HexToColor1 = [](Hex::Tile t) {
        switch(t) {
        case Hex::Tile::Blue:
            return UI::TileColor::Blue;
        case Hex::Tile::Red:
            return UI::TileColor::Red;
        default:
            return UI::TileColor::Empty;
        }
    };
    auto HexToColor2 = [](Hex::Tile t) {
        switch(t) {
        case Hex::Tile::Blue:
            return UI::TileColor::LightRed;
        case Hex::Tile::Red:
            return UI::TileColor::LightBlue;
        default:
            return UI::TileColor::Empty;
        }
    };

    for (size_t i = 0; i < tiles.size(); i++) {
        tiles.at(i).color = HexToColor1(hexTiles.at(i));
    }

    

    std::function<void(std::shared_ptr<HexTreeNode>)> futureTiles = [tiles, HexToColor2, &futureTiles](std::shared_ptr<HexTreeNode> n) mutable {
        auto parent = n->GetParent();
        if (parent != nullptr) {
            tiles.at(n->GetTile()).color = HexToColor2(n->GetHexTile());
            futureTiles(parent);
        }
    };

    emit BoardState(tiles);

    // Create Eval Tree
    UI::TileTree tileTree;
    tileTree.tilesBeforeEval.reserve(tiles.size());
    for (auto& i : tree->GetTiles()) {
        tileTree.tilesBeforeEval.push_back(HexToColor1(i));
    }

    std::function<std::shared_ptr<UI::TileNode>(std::shared_ptr<HexTreeNode>)> HexNodeToTileNode = [HexToColor2, &HexNodeToTileNode](std::shared_ptr<HexTreeNode> hexNode) -> std::shared_ptr<UI::TileNode> {
        if (hexNode == nullptr) {
            return nullptr;
        }
        auto tileNode = std::make_shared<UI::TileNode>();
        tileNode->score = hexNode->GetScore();
        tileNode->heuristicScore = hexNode->GetHeuristicScore();
        auto children = hexNode->GetChildren();
        tileNode->numChildren = static_cast<int>(children.size());
        tileNode->tileColor = HexToColor2(hexNode->GetHexTile());
        tileNode->tile = hexNode->GetTile();
        for (auto& i : children) {
            auto childTileNode = HexNodeToTileNode(i);
            if (childTileNode != nullptr) {
                tileNode->children.push_back(childTileNode);
                childTileNode->parent = tileNode;
                tileNode->numChildren += childTileNode->numChildren;
            }
        }

        return tileNode;
    };
    tileTree.root = HexNodeToTileNode(tree->GetRoot());
    if (lastMove != nullptr) {
        tileTree.lastMove = lastMove->GetTile();
        tileTree.lastMoveRed = lastMove->IsRed();
    }
    else {
        tileTree.lastMove = -1;
        tileTree.lastMoveRed = true;
    }
    emit EvalTree(tileTree);

}

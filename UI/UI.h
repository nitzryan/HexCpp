#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include <memory>

namespace UI
{
    enum TileColor
    {
        Red,
        Blue,
        LightRed,
        LightBlue,
        HighlightRed,
        HighlightBlue,
        Empty,
    };

    inline bool TileIsBlue(TileColor tc) {
        return tc == TileColor::Blue ||
                tc == TileColor::LightBlue ||
                tc == TileColor::HighlightBlue;
    }

    struct Tile
    {
        std::string text;
        TileColor color = TileColor::Empty;
    };

    struct TileNode
    {
        float score = 0;
        float heuristicScore = 0;
        int numChildren = -1;
        short tile = -1;
        TileColor tileColor = TileColor::Empty;
        std::vector<std::shared_ptr<TileNode>> children;
        std::weak_ptr<TileNode> parent;
    };

    struct TileTree
    {
        std::vector<TileColor> tilesBeforeEval;
        std::shared_ptr<TileNode> root;
        short lastMove;
        bool lastMoveRed;
    };
}

#endif // UI_H

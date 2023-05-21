#ifndef HEXCONTROLLER_H
#define HEXCONTROLLER_H

#include <QObject>

#include "../Game/Hex/HexGame.h"
#include <vector>
#include "UI.h"
#include "../Game/Hex/HexTreeData.h"

class HexController : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief HexController
     */
    HexController(QObject* parent = nullptr);
    ~HexController();

    /**
     * @brief BoardstateCallback
     * @param newBoard
     */
    void BoardstateCallback(std::unique_ptr<AbstractTreeData> newBoardTree, std::unique_ptr<AbstractMove> move);
public slots:
    void TileClicked(int tile);
    void NewGameRequested(int size, float time, bool P1AI, bool P2AI);

signals:
    void BoardState(std::vector<UI::Tile> tiles);
    void BoardMoves(std::vector<std::string> moves);
    void EvalTree(UI::TileTree tt);

private:
    std::unique_ptr<HexGame> game;
    std::unique_ptr<HexTreeData> tree;
    std::unique_ptr<HexMove> lastMove;

    void EmitTreeSignals(std::shared_ptr<HexTreeNode> node);
};

#endif // HEXCONTROLLER_H

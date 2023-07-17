#include "HexCpp.h"

HexCpp::HexCpp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    controller = new HexController();
    connect(ui.hexGrid, &HexGrid::tileClicked, controller, &HexController::TileClicked);
    connect(controller, &HexController::BoardState, ui.hexGrid, &HexGrid::BoardState);
    connect(ui.evalTree, &EvalTree::BoardTiles, ui.hexGrid, &HexGrid::BoardState);
    connect(controller, &HexController::EvalTree, ui.evalTree, &EvalTree::UpdateEvalTree);
    connect(ui.controlBar, &ControlBar::gameRequested, controller, &HexController::NewGameRequested);
    connect(controller, &HexController::BoardMoves, ui.moveViewer, &MoveViewer::SetMoveList);
    connect(ui.moveViewer, &MoveViewer::MoveSelected, controller, &HexController::MoveSelected);
    connect(ui.moveViewer, &MoveViewer::EvaluatePosition, controller, &HexController::EvaluatePosition);
    connect(ui.moveViewer, &MoveViewer::CommitPosition, controller, &HexController::CommitPosition);
}

HexCpp::~HexCpp()
{
    delete controller;
}

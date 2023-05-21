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
}

HexCpp::~HexCpp()
{
    delete controller;
}

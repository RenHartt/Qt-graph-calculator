#include "MainWindow.h"
#include "Calculator.h"
#include "GraphRenderer.h"
#include "EquationSolver.h"

void MainWindow::setupCalculator() {
  Calculator *calculator = new Calculator(this);
  _tabWidget->addTab(calculator, "Calculator");
}

void MainWindow::setupGraphRenderer() {
  GraphRenderer *graphRenderer = new GraphRenderer(this);
  _tabWidget->addTab(graphRenderer, "Graph");
}

void MainWindow::setupEquationSolver() {
  EquationSolver *equationSolver = new EquationSolver(this);
  _tabWidget->addTab(equationSolver, "Solver");
}

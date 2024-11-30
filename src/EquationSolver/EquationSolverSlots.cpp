#include "EquationSolver.h"
#include <QMessageBox>

void EquationSolver::onSolveClicked() {
  QString equation = _equationInput->text().simplified();

  if (equation.isEmpty()) {
    QMessageBox::warning(this, "Input Error", "Please enter an equation to solve.");
    return;
  }

  try {
    QString solution = solveEquation(equation);
    _solutionOutput->setText(solution);
  } catch (std::exception &e) {
    QMessageBox::warning(this, "Error", QString("An error occurred: %1").arg(e.what()));
  }
}

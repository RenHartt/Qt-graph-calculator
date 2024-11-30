#include "EquationSolver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

EquationSolver::EquationSolver(QWidget *parent)
    : QWidget(parent) {
  setupLayout();
}

void EquationSolver::setupLayout() {
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  QHBoxLayout *inputLayout = new QHBoxLayout;
  QLabel *label = new QLabel("Equation:", this);
  _equationInput = new QLineEdit(this);
  QPushButton *solveButton = new QPushButton("Solve", this);

  connect(solveButton, &QPushButton::clicked, this, &EquationSolver::onSolveClicked);

  inputLayout->addWidget(label);
  inputLayout->addWidget(_equationInput);
  inputLayout->addWidget(solveButton);

  mainLayout->addLayout(inputLayout);

  _solutionOutput = new QTextEdit(this);
  _solutionOutput->setReadOnly(true);
  mainLayout->addWidget(_solutionOutput);
}

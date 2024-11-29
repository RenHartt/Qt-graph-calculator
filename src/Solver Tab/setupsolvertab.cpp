#include <mainwindow.h>

void MainWindow::setupSolverTab() {
  QWidget *solverTab = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout;

  QLabel *equationLabel = new QLabel("Enter an expression :", this);
  mainLayout->addWidget(equationLabel);

  equationInput = new QLineEdit(this);
  mainLayout->addWidget(equationInput);

  QPushButton *solveButton = new QPushButton("Solve", this);
  mainLayout->addWidget(solveButton);

  solutionOutput = new QTextEdit(this);
  solutionOutput->setReadOnly(true);
  mainLayout->addWidget(solutionOutput);

  connect(solveButton, &QPushButton::clicked, this, &MainWindow::onSolveEquation);

  solverTab->setLayout(mainLayout);
  tabWidget->addTab(solverTab, "Equation solver");
}

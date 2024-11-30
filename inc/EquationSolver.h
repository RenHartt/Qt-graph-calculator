#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

class EquationSolver : public QWidget {
  Q_OBJECT

public:
  explicit EquationSolver(QWidget *parent = nullptr);
  ~EquationSolver() = default;

private slots:
  void onSolveClicked();

private:
  void setupLayout();
  QString solveEquation(const QString &equation);

  QLineEdit *_equationInput;
  QTextEdit *_solutionOutput;
};

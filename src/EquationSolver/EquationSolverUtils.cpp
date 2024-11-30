#include "EquationSolver.h"
#include <cmath>
#include <stdexcept>
#include <QRegularExpression>

QString EquationSolver::solveEquation(const QString &equation) {
  QRegularExpression regex(R"(([+-]?\d*\.?\d*)\*?x\s*([+-]\s*\d*\.?\d*)?\s*=\s*([+-]?\d*\.?\d*))");
  QRegularExpressionMatch match = regex.match(equation);

  if (!match.hasMatch()) {
    throw std::invalid_argument("Invalid equation format.");
  }

  double a = match.captured(1).isEmpty() ? 1 : match.captured(1).toDouble();
  double b = match.captured(2).isEmpty() ? 0 : match.captured(2).replace(" ", "").toDouble();
  double c = match.captured(3).toDouble();

  if (a == 0) {
    if (b == c)
      return "Infinite solutions.";
    else
      return "No solution.";
  }

  double x = (c - b) / a;
  return QString("x = %1").arg(x);
}

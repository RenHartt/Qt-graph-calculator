#include <mainwindow.h>

#include <stdexcept>

void processEquationSide(const QString &side, int sign, double &variableCoefficient, double &constantTerm) {
  QStringList terms = side.split(QRegExp("(?=[+-])"), Qt::SkipEmptyParts);
  for (const QString &term : terms) {
    QString trimmedTerm = term.trimmed();

    if (trimmedTerm.isEmpty())
      throw std::invalid_argument("Invalid term detected in the equation.");

    if (trimmedTerm.contains(QRegExp("[a-zA-Z]"))) {
      QRegExp variableRegex("([+-]?\\d*(\\.\\d+)?)?[a-zA-Z]");
      if (!variableRegex.exactMatch(trimmedTerm))
        throw std::invalid_argument(QString("Invalid variable term: '%1'").arg(trimmedTerm).toStdString());

      QString coefficientPart = variableRegex.cap(1);
      double coeff = coefficientPart.isEmpty() ? 1.0 : coefficientPart.toDouble();
      variableCoefficient += coeff * sign;

    } else {
      bool ok;
      double constant = trimmedTerm.toDouble(&ok);
      if (!ok)
        throw std::invalid_argument(QString("Invalid constant term: '%1'").arg(trimmedTerm).toStdString());
      constantTerm += constant * sign;
    }
  }
}

void MainWindow::onSolveEquation() {
  QString equationText = equationInput->text().simplified().remove(" ");

  try {
    if (equationText.isEmpty()) 
      throw std::invalid_argument("The equation cannot be empty.");

    QStringList parts = equationText.split('=');
    if (parts.size() != 2)
      throw std::invalid_argument("Equation must contain exactly one '='.");

    QString leftSide = parts[0].trimmed();
    QString rightSide = parts[1].trimmed();

    if (leftSide.isEmpty() || rightSide.isEmpty())
      throw std::invalid_argument("Both sides of the equation must be non-empty.");

    double variableCoefficient = 0;
    double constantTerm = 0;

    processEquationSide(leftSide, 1, variableCoefficient, constantTerm);
    processEquationSide(rightSide, -1, variableCoefficient, constantTerm);

    QString result;
    if (variableCoefficient == 0) {
		constantTerm == 0 ? result = "The equation is always true."
						  : result = "The equation is impossible.";
    } else {
      double solution = -constantTerm / variableCoefficient;
      result = QString("Solution: x = %1").arg(solution);
    }

    solutionOutput->setText(result);

  } catch (const std::exception &e) {
    solutionOutput->setText(QString("Error: %1").arg(e.what()));
  }
}

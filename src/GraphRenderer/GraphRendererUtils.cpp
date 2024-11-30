#include "GraphRenderer.h"
#include <cmath>
#include <stdexcept>
#include <QRegularExpression>

double calculateRecursive(const QString &expression);

bool calculateNumber(const QString &expression) {
  bool isNumber;
  expression.toDouble(&isNumber);
  return isNumber;
}

bool calculateParentheses(QString &expression) {
  QRegularExpression parentheses("\\(([^()]+)\\)");
  QRegularExpressionMatch match = parentheses.match(expression);

  if (match.hasMatch()) {
    QString innerExpression = match.captured(1);
    double innerResult = calculateRecursive(innerExpression);

    expression.replace(match.captured(0), QString::number(innerResult));
    return true;
  }
  return false;
}

bool calculatePower(QString &expression) {
  QRegularExpression powerOp(
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)\\s*\\^\\s*"
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)");
  QRegularExpressionMatch match = powerOp.match(expression);

  if (match.hasMatch()) {
    double base = match.captured(1).toDouble();
    double exponent = match.captured(2).toDouble();

    if (base < 0 && exponent != static_cast<int>(exponent)) {
      throw std::domain_error("Fractional powers of negative bases are not supported.");
    }

    double result = pow(base, exponent);
    expression.replace(match.captured(0), QString::number(result, 'g', 15));
    return true;
  }
  return false;
}

bool calculatePriorityOps(QString &expression) {
  QRegularExpression priorityOps(
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)\\s*([*/%])\\s*"
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)");
  QRegularExpressionMatch match = priorityOps.match(expression);

  if (match.hasMatch()) {
    double left = match.captured(1).toDouble();
    QString op = match.captured(2);
    double right = match.captured(3).toDouble();

    if (op == "/" && right == 0)
      throw std::domain_error("Division by zero.");

    if (op == "%" && right == 0)
      throw std::domain_error("Modulo by zero.");

    double result = 0;
    if (op == "*") result = left * right;
    else if (op == "/") result = left / right;
    else if (op == "%") result = fmod(left, right);

    expression.replace(match.captured(0), QString::number(result));
    return true;
  }
  return false;
}

bool calculateBasicOps(QString &expression) {
  QRegularExpression basicOps(
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)\\s*([+-])\\s*"
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)");
  QRegularExpressionMatch match = basicOps.match(expression);

  if (match.hasMatch()) {
    double left = match.captured(1).toDouble();
    QString op = match.captured(2);
    double right = match.captured(3).toDouble();

    double result = (op == "+") ? left + right : left - right;
    expression.replace(match.captured(0), QString::number(result));
    return true;
  }
  return false;
}

double calculateRecursive(const QString &expression) {
  QString expr = expression;

  if (calculateNumber(expr))
    return expr.toDouble();

  if (calculateParentheses(expr))
    return calculateRecursive(expr);

  if (calculatePower(expr))
    return calculateRecursive(expr);

  if (calculatePriorityOps(expr))
    return calculateRecursive(expr);

  if (calculateBasicOps(expr))
    return calculateRecursive(expr);

  throw std::invalid_argument("Invalid expression.");
}

double GraphRenderer::evaluateExpression(const QString &expression, double xValue) {
  QString parsedExpression = expression.simplified();
  parsedExpression.replace("x", QString::number(xValue));

  QRegularExpression validExpr("^[0-9.\\-+*/%^eE\\(\\) ]+$");
  if (!validExpr.match(parsedExpression).hasMatch()) {
    throw std::invalid_argument("Invalid expression format.");
  }

  return calculateRecursive(parsedExpression);
}

int GraphRenderer::calculateResolution() {
  return static_cast<int>(std::min(1000.0, 100 * (_xMax - _xMin)));
}

void GraphRenderer::updateGraph() {
  for (int i = 0; i < _customPlot->graphCount(); ++i) {
    QString expression = _expressionList->item(i)->text();
    QVector<double> x, y;

    int resolution = calculateResolution();
    x.resize(resolution);
    y.resize(resolution);

    for (int j = 0; j < resolution; ++j) {
      double xValue = _xMin + j * (_xMax - _xMin) / (resolution - 1);
      x[j] = xValue;

      try {
        y[j] = evaluateExpression(expression, xValue);
      } catch (std::exception &) {
        y[j] = 0;
      }
    }

    _customPlot->graph(i)->setData(x, y);
  }

  _customPlot->xAxis->setRange(_xMin, _xMax);
  _customPlot->yAxis->setRange(_yMin, _yMax);
  _customPlot->replot();
}

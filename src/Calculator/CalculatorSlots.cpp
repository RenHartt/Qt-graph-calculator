#include "Calculator.h"
#include <QPushButton>
#include <cmath>

void Calculator::onDigitClicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (!button)
    return;

  QString digit = button->text();
  if (_display->text() == "0")
    _display->setText(digit);
  else
    _display->setText(_display->text() + digit);
}

void Calculator::onOperatorClicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (!button)
    return;

  _pendingOperator = button->text();
  _leftOperand = _display->text();
  _display->setText("0");
  _operatorPending = true;
}

void Calculator::onDotClicked() {
  if (!_display->text().contains('.')) {
    _display->setText(_display->text() + ".");
  }
}

void Calculator::onEqualClicked() {
  if (!_operatorPending)
    return;

  _rightOperand = _display->text();
  double left = _leftOperand.toDouble();
  double right = _rightOperand.toDouble();
  double result = 0;

  if (_pendingOperator == "+")
    result = left + right;
  else if (_pendingOperator == "-")
    result = left - right;
  else if (_pendingOperator == "*")
    result = left * right;
  else if (_pendingOperator == "/") {
    if (right == 0) {
      _display->setText("Error");
      return;
    }
    result = left / right;
  }

  _display->setText(QString::number(result, 'g', 15));
  _operatorPending = false;
}

void Calculator::onClearClicked() {
  _display->setText("0");
  _pendingOperator.clear();
  _leftOperand.clear();
  _rightOperand.clear();
  _operatorPending = false;
}

void Calculator::onScientificFunctionClicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (!button)
    return;

  QString function = button->text();
  applyScientificFunction(function);
}

void Calculator::applyScientificFunction(const QString &function) {
  double value = _display->text().toDouble();

  if (function == "sin")
    value = std::sin(value);
  else if (function == "cos")
    value = std::cos(value);
  else if (function == "tan")
    value = std::tan(value);
  else if (function == "log")
    value = std::log10(value);
  else if (function == "ln")
    value = std::log(value);
  else if (function == "sqrt")
    value = std::sqrt(value);
  else if (function == "exp")
    value = std::exp(value);
  else if (function == "pow")
    value = std::pow(value, 2);

  _display->setText(QString::number(value, 'g', 15));
}

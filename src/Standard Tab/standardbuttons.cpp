#include "mainwindow.h"

void MainWindow::onDigitClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (!clickedButton)
    return;

  QString buttonText = clickedButton->text();

  displayStandard->text() == "0"
      ? displayStandard->setText(buttonText)
      : displayStandard->setText(displayStandard->text() + buttonText);
  displayScientific->text() == "0"
      ? displayScientific->setText(buttonText)
      : displayScientific->setText(displayScientific->text() + buttonText);
}

void MainWindow::onOperatorClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (!clickedButton)
    return;

  QString buttonText = clickedButton->text();

  if (!operatorPending) {
    leftOperand = displayStandard->text();
    pendingOperator = buttonText;
    operatorPending = true;
    displayStandard->setText("0");
    displayScientific->setText("0");
  }
}

void MainWindow::onEqualClicked() {
  if (leftOperand.isEmpty() || pendingOperator.isEmpty())
    return;

  rightOperand = displayStandard->text();

  double left = leftOperand.toDouble();
  double right = rightOperand.toDouble();
  double result = 0;

  if (pendingOperator == "+")
    result = left + right;
  else if (pendingOperator == "-")
    result = left - right;
  else if (pendingOperator == "*")
    result = left * right;
  else if (pendingOperator == "/") {
    if (right == 0) {
      displayStandard->setText("Error");
      displayScientific->setText("Error");
      return;
    }
    result = left / right;
  }

  displayStandard->setText(QString::number(result));
  displayScientific->setText(QString::number(result));

  leftOperand = QString::number(result);
  rightOperand.clear();
  pendingOperator.clear();
  operatorPending = false;
}

void MainWindow::onClearClicked() {
  displayStandard->setText("0");
  displayScientific->setText("0");
  leftOperand.clear();
  rightOperand.clear();
  pendingOperator.clear();
  operatorPending = false;
}


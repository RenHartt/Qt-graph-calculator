#include <QtMath>
#include <mainwindow.h>

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

void MainWindow::onScientificClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (!clickedButton)
    return;

  QString func = clickedButton->text();

  double value = displayStandard->text().toDouble();

  if (func == "sin")
    value = sin(value);
  else if (func == "cos")
    value = cos(value);
  else if (func == "tan") {
    if (fmod(value, M_PI_2) == 0) {
      displayStandard->setText("Error");
      displayScientific->setText("Error");
      return;
    }
    value = tan(value);
  } else if (func == "sqrt") {
    if (value < 0) {
      displayStandard->setText("Error");
      displayScientific->setText("Error");
      return;
    }
    value = sqrt(value);
  } else if (func == "log") {
    if (value <= 0) {
      displayStandard->setText("Error");
      displayScientific->setText("Error");
      return;
    }
    value = log(value);
  } else if (func == "exp")
    value = exp(value);
  else if (func == "pi")
    value = M_PI;

  displayStandard->setText(QString::number(value));
  displayScientific->setText(QString::number(value));
}

void MainWindow::onTraceClicked() {
  xMin = xMinInput->text().toDouble();
  xMax = xMaxInput->text().toDouble();
  yMin = yMinInput->text().toDouble();
  yMax = yMaxInput->text().toDouble();

  if (xMin >= xMax || yMin >= yMax) {
    QMessageBox::warning(this, "Error", "Invalid ranges");
    return;
  }

  updateGraph();
}

void MainWindow::onAddExpression() {
  QString expression = expressionInput->text().simplified();
  if (expression.isEmpty()) {
    QMessageBox::warning(this, "Erreur", "Veuillez entrer une expression.");
    return;
  }

  expressionList->addItem(expression);

  customPlot->addGraph();
  customPlot->graph(customPlot->graphCount() - 1)->setName(expression);

  updateGraph();
}

void MainWindow::onDelExpression() {
  QListWidgetItem *currentItem = expressionList->currentItem();
  if (!currentItem) {
    QMessageBox::warning(this, "Erreur",
                         "Veuillez sélectionner une expression à supprimer.");
    return;
  }

  int index = expressionList->row(currentItem);

  delete expressionList->takeItem(index);

  customPlot->removeGraph(index);
  customPlot->replot();
}

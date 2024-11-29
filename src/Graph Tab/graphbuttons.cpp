#include "mainwindow.h"

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
  if (expression.isEmpty())
	return;

  QListWidgetItem *newItem = new QListWidgetItem(expression, expressionList);
  newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
  expressionList->addItem(newItem);

  expressionInput->clear();

  customPlot->addGraph();
  int graphIndex = customPlot->graphCount() - 1;
  QColor color = QColor::fromHsv((graphIndex * 50) % 360, 255, 200);
  customPlot->graph(graphIndex)->setName(expression);
  customPlot->graph(graphIndex)->setPen(QPen(color));

  updateGraph();
}

void MainWindow::onDelExpression() {
  QListWidgetItem *currentItem = expressionList->currentItem();
  if (!currentItem)
	return;

  int index = expressionList->row(currentItem);
  delete expressionList->takeItem(index);
  customPlot->removeGraph(index);

  updateGraph();
}

void MainWindow::onExpressionEdited(QListWidgetItem *item) {
  if (!item)
	return;

  int index = expressionList->row(item);
  if (index < 0 || index >= customPlot->graphCount())
	return;

  try {
	QVector<double> x(1001), y(1001);
	double xMin = xMinInput->text().toDouble();
	double xMax = xMaxInput->text().toDouble();

	for (int j = 0; j < x.size(); ++j) {
	  x[j] = xMin + j * (xMax - xMin) / (x.size() - 1);
	  y[j] = evaluateExpression(item->text(), x[j]);
	}

	customPlot->graph(index)->setData(x, y);
	customPlot->replot();
  } catch (std::exception &e) {
	QMessageBox::warning(this, "Erreur", QString("Expression invalide : %1").arg(e.what()));
  }
}


#include "GraphRenderer.h"
#include <QMessageBox>

void GraphRenderer::onTraceClicked() {
  _xMin = _xMinInput->text().toDouble();
  _xMax = _xMaxInput->text().toDouble();
  _yMin = _yMinInput->text().toDouble();
  _yMax = _yMaxInput->text().toDouble();

  if (_xMin >= _xMax || _yMin >= _yMax) {
    QMessageBox::warning(this, "Invalid Range", "Ensure X Min < X Max and Y Min < Y Max.");
    return;
  }

  updateGraph();
}

void GraphRenderer::onAddExpression() {
  QString expression = _expressionInput->text().simplified();
  if (expression.isEmpty())
    return;

  QListWidgetItem *newItem = new QListWidgetItem(expression, _expressionList);
  newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);

  _customPlot->addGraph();
  int graphIndex = _customPlot->graphCount() - 1;
  QColor color = QColor::fromHsv((graphIndex * 50) % 360, 255, 200);
  _customPlot->graph(graphIndex)->setName(expression);
  _customPlot->graph(graphIndex)->setPen(QPen(color));

  _expressionInput->clear();
  updateGraph();
}

void GraphRenderer::onDelExpression() {
  QListWidgetItem *currentItem = _expressionList->currentItem();
  if (!currentItem)
    return;

  int index = _expressionList->row(currentItem);
  delete _expressionList->takeItem(index);
  _customPlot->removeGraph(index);

  updateGraph();
}

void GraphRenderer::onExpressionEdited(QListWidgetItem *item) {
  if (!item)
    return;

  updateGraph();
}

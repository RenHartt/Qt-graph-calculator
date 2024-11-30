#pragma once

#include "../inc/qcustomplot.h"
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>

class GraphRenderer : public QWidget {
  Q_OBJECT

public:
  explicit GraphRenderer(QWidget *parent = nullptr);
  ~GraphRenderer() = default;

private slots:
  void onTraceClicked();
  void onAddExpression();
  void onDelExpression();
  void onExpressionEdited(QListWidgetItem *item);

private:
  void setupLayout();
  void updateGraph();
  double evaluateExpression(const QString &expression, double xValue);
  int calculateResolution();

  QCustomPlot *_customPlot;
  QLineEdit *_xMinInput;
  QLineEdit *_xMaxInput;
  QLineEdit *_yMinInput;
  QLineEdit *_yMaxInput;
  QListWidget *_expressionList;
  QLineEdit *_expressionInput;

  double _xMin;
  double _xMax;
  double _yMin;
  double _yMax;
};

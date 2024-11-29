#pragma once

#include "../inc/qcustomplot.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <qchar.h>
#include <qtabwidget.h>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onDigitClicked();
  void onOperatorClicked();
  void onEqualClicked();
  void onClearClicked();
  void onScientificClicked();

  void onTraceClicked();
  void onAddExpression();
  void onDelExpression();
  void onExpressionEdited(QListWidgetItem *item);

  void onSolveEquation();

private:
  void setupStandardTab();
  void setupScientificTab();
  void setupGraphTab();
  void setupSolverTab();

  void updateGraph();
  double evaluateExpression(const QString &expression, double xValue);

  QTabWidget *tabWidget;
  QLineEdit *displayStandard;
  QLineEdit *displayScientific;
  QCustomPlot *customPlot;

  QString pendingOperator;
  QString leftOperand;
  QString rightOperand;
  bool operatorPending;

  QLineEdit *xMinInput;
  QLineEdit *xMaxInput;
  QLineEdit *yMinInput;
  QLineEdit *yMaxInput;

  double xMin;
  double xMax;
  double yMin;
  double yMax;

  QListWidget *expressionList;
  QLineEdit *expressionInput;

  QLineEdit *equationInput;
  QTextEdit *solutionOutput;
};

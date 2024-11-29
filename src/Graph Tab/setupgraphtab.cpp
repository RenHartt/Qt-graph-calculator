#include <QRegularExpression>
#include <QStack>
#include <QString>
#include <QStringList>
#include <cmath>
#include <mainwindow.h>
#include <qboxlayout.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <qtextcursor.h>
#include <stdexcept>

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
    double left = match.captured(1).toDouble();
    double right = match.captured(2).toDouble();
    qDebug() << "Base:" << left;
    qDebug() << "Exponent:" << right;
    double result;

    if (qFuzzyCompare(
            right, static_cast<int>(right))) { // Évite les erreurs flottantes
      if (left < 0 && static_cast<int>(right) % 2 != 0) {
        result = -pow(-left, right);
      } else {
        result = pow(left, right);
      }
    } else {
      if (left < 0) {
        throw std::domain_error(
            "Puissance fractionnaire d'une base négative non supportée.");
      }
      result = pow(left, right);
    }

    // Remplacement de l'expression par le résultat
    expression.replace(match.captured(0), QString::number(result, 'g', 15));
    return true;
  }
  return false;
}

bool calculatePriorityOps(QString &expression) {
  QRegularExpression priorityOps(
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)\\s*([*/"
      "%])\\s*(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)");
  QRegularExpressionMatch match = priorityOps.match(expression);

  if (match.hasMatch()) {
    double left = match.captured(1).toDouble();
    QString op = match.captured(2);
    double right = match.captured(3).toDouble();

    double result = 0;
    if (op == "*")
      result = left * right;
    else if (op == "/") {
      if (right == 0)
        throw std::domain_error("Zero division.");
      result = left / right;
    } else if (op == "%") {
      if (right == 0)
        throw std::domain_error("Zero modulo.");
      result = fmod(left, right);
    }

    expression.replace(match.captured(0), QString::number(result));
    return true;
  }
  return false;
}

bool calculateBasicOps(QString &expression) {
  QRegularExpression basicOps(
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)\\s*([+-])"
      "\\s*(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)");
  QRegularExpressionMatch match = basicOps.match(expression);

  if (match.hasMatch()) {
    double left = match.captured(1).toDouble();
    QString op = match.captured(2);
    double right = match.captured(3).toDouble();

    double result = 0;
    if (op == "+")
      result = left + right;
    else if (op == "-")
      result = left - right;

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

  throw std::invalid_argument("Non valid expression.");
}

double MainWindow::evaluateExpression(const QString &expression,
                                      double xValue) {
  QString parsedExpression = expression.simplified();
  parsedExpression.replace("x", QString::number(xValue));

  QRegularExpression validExpr("^[0-9.\\-+*/%^eE\\(\\) ]+$");
  if (!validExpr.match(parsedExpression).hasMatch())
    throw std::invalid_argument("Invalid expression.");

  return calculateRecursive(parsedExpression);
}

void MainWindow::updateGraph() {
  int graphCount = customPlot->graphCount();
  for (int i = 0; i < graphCount; ++i) {
    QString expression = expressionList->item(i)->text();
    if (expression.isEmpty())
      continue;

    QVector<double> x(1001), y(1001);
    double xMin = xMinInput->text().toDouble();
    double xMax = xMaxInput->text().toDouble();

    for (int j = 0; j < x.size(); ++j) {
      x[j] = xMin + j * (xMax - xMin) / (x.size() - 1);
      try {
        y[j] = evaluateExpression(expression, x[j]);
      } catch (std::exception &e) {
        y[j] = 0;
      }
    }
    customPlot->graph(i)->setData(x, y);
  }

  double yMin = yMinInput->text().toDouble();
  double yMax = yMaxInput->text().toDouble();
  customPlot->xAxis->setRange(xMinInput->text().toDouble(),
                              xMaxInput->text().toDouble());
  customPlot->yAxis->setRange(yMin, yMax);

  customPlot->replot();
}

void MainWindow::setupGraphTab() {
  QWidget *graphTab = new QWidget(this);
  QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
  QWidget *leftWidget = new QWidget(this);
  QVBoxLayout *leftLayout = new QVBoxLayout;
  QHBoxLayout *axisRangeLayout = new QHBoxLayout;

  xMinInput = new QLineEdit(this);
  xMinInput->setText("-10");
  xMinInput->setPlaceholderText("X min");
  axisRangeLayout->addWidget(new QLabel("X Min:", this));
  axisRangeLayout->addWidget(xMinInput);

  xMaxInput = new QLineEdit(this);
  xMaxInput->setText("10");
  xMaxInput->setPlaceholderText("X max");
  axisRangeLayout->addWidget(new QLabel("X Max:", this));
  axisRangeLayout->addWidget(xMaxInput);

  yMinInput = new QLineEdit(this);
  yMinInput->setText("-10");
  yMinInput->setPlaceholderText("Y min");
  axisRangeLayout->addWidget(new QLabel("Y Min:", this));
  axisRangeLayout->addWidget(yMinInput);

  yMaxInput = new QLineEdit(this);
  yMaxInput->setText("10");
  yMaxInput->setPlaceholderText("Y max");
  axisRangeLayout->addWidget(new QLabel("Y Max:", this));
  axisRangeLayout->addWidget(yMaxInput);

  leftLayout->addLayout(axisRangeLayout);

  expressionInput = new QLineEdit(this);
  expressionInput->setPlaceholderText("Enter an expression");
  leftLayout->addWidget(expressionInput);

  customPlot = new QCustomPlot(this);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
  customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
  customPlot->axisRect()->setRangeZoomFactor(0.8, 0.8);
  customPlot->legend->setVisible(true);
  customPlot->legend->setBrush(QBrush(Qt::white));
  customPlot->xAxis->setLabel("X");
  customPlot->yAxis->setLabel("Y");
  customPlot->xAxis->grid()->setVisible(true);
  customPlot->yAxis->grid()->setVisible(true);

  leftLayout->addWidget(customPlot);

  QPushButton *plotButton = new QPushButton("Tracer", this);
  leftLayout->addWidget(plotButton);
  connect(plotButton, &QPushButton::clicked, this,
          &MainWindow::onAddExpression);

  leftWidget->setLayout(leftLayout);

  QWidget *rightWidget = new QWidget(this);
  QVBoxLayout *rightLayout = new QVBoxLayout;

  QLabel *label = new QLabel("Expressions :", this);
  rightLayout->addWidget(label);

  expressionList = new QListWidget(this);
  expressionList->setEditTriggers(QAbstractItemView::DoubleClicked |
                                  QAbstractItemView::EditKeyPressed);
  expressionList->setSelectionMode(QAbstractItemView::SingleSelection);
  expressionList->setFocusPolicy(Qt::StrongFocus);
  connect(expressionList, &QListWidget::itemChanged, this,
          &MainWindow::updateGraph);
  rightLayout->addWidget(expressionList);

  QPushButton *removeButton = new QPushButton("Delete", this);
  rightLayout->addWidget(removeButton);
  connect(removeButton, &QPushButton::clicked, this,
          &MainWindow::onDelExpression);

  rightWidget->setLayout(rightLayout);

  splitter->addWidget(leftWidget);
  splitter->addWidget(rightWidget);

  splitter->setStretchFactor(0, 7);
  splitter->setStretchFactor(1, 1);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(splitter);

  graphTab->setLayout(mainLayout);

  tabWidget->addTab(graphTab, "Graphic");
}

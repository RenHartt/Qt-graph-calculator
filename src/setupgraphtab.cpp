#include <QRegularExpression>
#include <QStack>
#include <QString>
#include <QStringList>
#include <cmath>
#include <cstddef>
#include <mainwindow.h>
#include <qboxlayout.h>
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
      "(-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)\\s*\\^\\s*("
      "-?(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)");
  QRegularExpressionMatch match = powerOp.match(expression);

  if (match.hasMatch()) {
    double left = match.captured(1).toDouble();
    double right = match.captured(2).toDouble();
    double result = pow(left, right);

    expression.replace(match.captured(0), QString::number(result));
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
        throw std::domain_error("Division par zéro.");
      result = left / right;
    } else if (op == "%") {
      if (right == 0)
        throw std::domain_error("Modulo par zéro.");
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

  throw std::invalid_argument("Expression invalide ou non prise en charge.");
}

double evaluateExpression(const QString &expression, double xValue) {
  QString parsedExpression = expression.simplified();
  parsedExpression.replace("x", QString::number(xValue));

  QRegularExpression validExpr("^[0-9.\\-+*/%^eE\\(\\) ]+$");
  if (!validExpr.match(parsedExpression).hasMatch())
    throw std::invalid_argument("Expression invalide.");

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
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QBrush(Qt::white));
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->yAxis->grid()->setVisible(true);

    leftLayout->addWidget(customPlot);

    QPushButton *plotButton = new QPushButton("Tracer", this);
    leftLayout->addWidget(plotButton);
    connect(plotButton, &QPushButton::clicked, this, &MainWindow::onAddExpression);

    leftWidget->setLayout(leftLayout);

    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout;

    QLabel *label = new QLabel("Expressions affichées :", this);
    rightLayout->addWidget(label);

    expressionList = new QListWidget(this);
    rightLayout->addWidget(expressionList);

    QPushButton *removeButton = new QPushButton("Supprimer", this);
    rightLayout->addWidget(removeButton);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onDelExpression);

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

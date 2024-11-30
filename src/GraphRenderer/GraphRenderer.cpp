#include "GraphRenderer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

GraphRenderer::GraphRenderer(QWidget *parent)
    : QWidget(parent), _xMin(-10), _xMax(10), _yMin(-10), _yMax(10) {
  setupLayout();
}

void GraphRenderer::setupLayout() {
  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  QVBoxLayout *leftLayout = new QVBoxLayout;
  QGroupBox *axesGroup    = new QGroupBox("Axes Range", this);
  QGridLayout *axesLayout = new QGridLayout;

  _xMinInput = new QLineEdit(QString::number(_xMin), this);
  _xMaxInput = new QLineEdit(QString::number(_xMax), this);
  _yMinInput = new QLineEdit(QString::number(_yMin), this);
  _yMaxInput = new QLineEdit(QString::number(_yMax), this);

  axesLayout->addWidget(new QLabel("X Min:"), 0, 0);
  axesLayout->addWidget(_xMinInput, 0, 1);
  axesLayout->addWidget(new QLabel("X Max:"), 1, 0);
  axesLayout->addWidget(_xMaxInput, 1, 1);
  axesLayout->addWidget(new QLabel("Y Min:"), 2, 0);
  axesLayout->addWidget(_yMinInput, 2, 1);
  axesLayout->addWidget(new QLabel("Y Max:"), 3, 0);
  axesLayout->addWidget(_yMaxInput, 3, 1);

  axesGroup->setLayout(axesLayout);
  leftLayout->addWidget(axesGroup);

  _expressionInput = new QLineEdit(this);
  _expressionInput->setPlaceholderText("Enter an expression");
  leftLayout->addWidget(_expressionInput);

  QHBoxLayout *buttonLayout = new QHBoxLayout;

  QPushButton *addButton = new QPushButton("Add", this);
  connect(addButton, &QPushButton::clicked, this, &GraphRenderer::onAddExpression);
  buttonLayout->addWidget(addButton);

  QPushButton *deleteButton = new QPushButton("Delete", this);
  connect(deleteButton, &QPushButton::clicked, this, &GraphRenderer::onDelExpression);
  buttonLayout->addWidget(deleteButton);

  leftLayout->addLayout(buttonLayout);

  _expressionList = new QListWidget(this);
  connect(_expressionList, &QListWidget::itemChanged, this, &GraphRenderer::onExpressionEdited);
  leftLayout->addWidget(_expressionList);

  mainLayout->addLayout(leftLayout, 1);

  _customPlot = new QCustomPlot(this);
  _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  _customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
  _customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
  mainLayout->addWidget(_customPlot, 3);
}

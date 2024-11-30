#include "Calculator.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <cmath>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), _operatorPending(false) {
  setupLayout();
}

QPushButton* Calculator::createButton(const QString &text) {
  QPushButton *button = new QPushButton(text, this);

  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  if (text == "+" || text == "-" || text == "*" || text == "/") {
    connect(button, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
  } else if (text == "=") {
    connect(button, &QPushButton::clicked, this, &Calculator::onEqualClicked);
  } else if (text == "C") {
    connect(button, &QPushButton::clicked, this, &Calculator::onClearClicked);
  } else if (text == ".") {
    connect(button, &QPushButton::clicked, this, &Calculator::onDotClicked);
  } else {
    connect(button, &QPushButton::clicked, this, &Calculator::onDigitClicked);
  }

  return button;
}

void Calculator::setupLayout() {
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  _display = new QLineEdit(this);
  _display->setReadOnly(true);
  _display->setAlignment(Qt::AlignRight);
  _display->setText("0");
  mainLayout->addWidget(_display);

  QGridLayout *standardLayout = new QGridLayout;

  standardLayout->addWidget(createButton("7"), 0, 0);
  standardLayout->addWidget(createButton("8"), 0, 1);
  standardLayout->addWidget(createButton("9"), 0, 2);
  standardLayout->addWidget(createButton("/"), 0, 3);
  standardLayout->addWidget(createButton("4"), 1, 0);
  standardLayout->addWidget(createButton("5"), 1, 1);
  standardLayout->addWidget(createButton("6"), 1, 2);
  standardLayout->addWidget(createButton("*"), 1, 3);
  standardLayout->addWidget(createButton("1"), 2, 0);
  standardLayout->addWidget(createButton("2"), 2, 1);
  standardLayout->addWidget(createButton("3"), 2, 2);
  standardLayout->addWidget(createButton("-"), 2, 3);
  standardLayout->addWidget(createButton("0"), 3, 0);
  standardLayout->addWidget(createButton("."), 3, 1);
  standardLayout->addWidget(createButton("="), 3, 2);
  standardLayout->addWidget(createButton("+"), 3, 3);

   mainLayout->addLayout(standardLayout);

  QGridLayout *scientificLayout = new QGridLayout;
  QStringList functions = {"sin", "cos", "tan", "log", "ln", "sqrt", "pow", "exp"};
  int row = 0, col = 0;
  for (const QString &func : functions) {
    QPushButton *button = new QPushButton(func, this);
    connect(button, &QPushButton::clicked, this, &Calculator::onScientificFunctionClicked);
    scientificLayout->addWidget(button, row, col);

    if (++col > 3) {
      col = 0;
      ++row;
    }
  }

  mainLayout->addLayout(scientificLayout);

  QHBoxLayout *additionalLayout = new QHBoxLayout;
  QPushButton *clearButton = new QPushButton("C", this);

  connect(clearButton, &QPushButton::clicked, this, &Calculator::onClearClicked);

  additionalLayout->addWidget(clearButton);

  mainLayout->addLayout(additionalLayout);
}

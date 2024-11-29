#include <mainwindow.h>

void MainWindow::setupStandardTab() {
  QWidget *standardTab = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout;

  displayStandard = new QLineEdit(this);
  displayStandard->setReadOnly(true);
  displayStandard->setAlignment(Qt::AlignRight);
  displayStandard->setText("0");
  layout->addWidget(displayStandard);

  QGridLayout *gridLayout = new QGridLayout;

  QStringList digits{"7", "8", "9", "4", "5", "6", "1", "2", "3", "0"};
  QStringList operators{"+", "-", "*", "/"};
  QPushButton *button;

  int position = 0;
  for (const QString &digit : digits) {
    button = new QPushButton(digit, this);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    int row = position / 3;
    int col = position % 3;
    gridLayout->addWidget(button, row, col);
    connect(button, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    position++;
  }

  int row = 0;
  for (const QString &op : operators) {
    button = new QPushButton(op, this);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gridLayout->addWidget(button, row++, 3);
    connect(button, &QPushButton::clicked, this,
            &MainWindow::onOperatorClicked);
  }

  QString clear = "c";
  button = new QPushButton(clear, this);
  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gridLayout->addWidget(button, 3, 1);
  connect(button, &QPushButton::clicked, this, &MainWindow::onClearClicked);

  QString equal = "=";
  button = new QPushButton(equal, this);
  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gridLayout->addWidget(button, 3, 2);
  connect(button, &QPushButton::clicked, this, &MainWindow::onEqualClicked);

  layout->addLayout(gridLayout);
  standardTab->setLayout(layout);
  tabWidget->addTab(standardTab, "Standard");
}

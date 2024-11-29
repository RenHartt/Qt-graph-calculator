#include <QtMath>
#include <mainwindow.h>

void MainWindow::setupScientificTab() {
  QWidget *scientificTab = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout;

  displayScientific = new QLineEdit(this);
  displayScientific->setReadOnly(true);
  displayScientific->setAlignment(Qt::AlignRight);
  displayScientific->setText("0");
  layout->addWidget(displayScientific);

  QGridLayout *gridLayout = new QGridLayout;

  QStringList functions{"sin", "cos", "tan", "sqrt", "log", "exp", "pi"};
  int position = 0;

  for (const QString &func : functions) {
    QPushButton *button = new QPushButton(func, this);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    int row = position / 4;
    int col = position % 4;
    gridLayout->addWidget(button, row, col);

    connect(button, &QPushButton::clicked, this, &MainWindow::onScientificClicked);

    position++;
  }

  QString equal = "=";
  QPushButton *button = new QPushButton(equal, this);
  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gridLayout->addWidget(button, 1, 3);
  connect(button, &QPushButton::clicked, this, &MainWindow::onEqualClicked);

  layout->addLayout(gridLayout);
  scientificTab->setLayout(layout);
  tabWidget->addTab(scientificTab, "Scientific");
}

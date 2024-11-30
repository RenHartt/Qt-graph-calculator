#include "MainWindow.h"
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _tabWidget(new QTabWidget(this)) {

  setupCalculator();
  setupGraphRenderer();
  setupEquationSolver();

  setCentralWidget(_tabWidget);
  setWindowTitle("Scientific Calculator");

  QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
  int width = 800;
  int height = 600;
  resize(width, height);
  move((screenGeometry.width() - width) / 2, (screenGeometry.height() - height) / 2);
}

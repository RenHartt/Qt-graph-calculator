#include "mainwindow.h"
#include <QApplication>
#include <QRect>
#include <QScreen>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qchar.h>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tabWidget(new QTabWidget(this)) {
  setupStandardTab();
  setupScientificTab();
  setupGraphTab();

  setCentralWidget(tabWidget);
  setWindowTitle("Scientific Calculator");
  resize(300, 200);
}

MainWindow::~MainWindow(void) {}

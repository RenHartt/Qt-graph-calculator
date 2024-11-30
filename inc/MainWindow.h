#pragma once

#include <QMainWindow>
#include <QTabWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() = default;

private:
  void setupCalculator();
  void setupGraphRenderer();
  void setupEquationSolver();

  QTabWidget *_tabWidget;
};

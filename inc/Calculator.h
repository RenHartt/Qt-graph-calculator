#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class Calculator : public QWidget {
  Q_OBJECT

public:
  explicit Calculator(QWidget *parent = nullptr);
  ~Calculator() = default;

private slots:
  void onDigitClicked();
  void onDotClicked();
  void onOperatorClicked();
  void onEqualClicked();
  void onClearClicked();
  void onScientificFunctionClicked();

private:
  QPushButton *createButton(const QString &text);
  void setupLayout();
  void applyOperator(const QString &op);
  void applyScientificFunction(const QString &function);

  QLineEdit *_display;
  QString _pendingOperator;
  QString _leftOperand;
  QString _rightOperand;
  bool _operatorPending;
};

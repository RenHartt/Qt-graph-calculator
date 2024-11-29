#include "mainwindow.h"

void MainWindow::onScientificClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (!clickedButton)
    return;

  QString func = clickedButton->text();
  double value = displayStandard->text().toDouble();

  if (func == "sin")
    value = sin(value);
  else if (func == "cos")
    value = cos(value);
  else if (func == "tan") {
    if (fmod(value, M_PI_2) == 0) {
      displayStandard->setText("Error");
      displayScientific->setText("Error");
      return;
    }
    value = tan(value);
  } else if (func == "sqrt") {
    if (value < 0) {
      displayStandard->setText("Error");
      displayScientific->setText("Error");
      return;
    }
    value = sqrt(value);
  } else if (func == "log") {
    if (value <= 0) {
      displayStandard->setText("Error");
      displayScientific->setText("Error");
      return;
    }
    value = log(value);
  } else if (func == "exp")
    value = exp(value);
  else if (func == "pi")
    value = M_PI;

  displayStandard->setText(QString::number(value));
  displayScientific->setText(QString::number(value));
}

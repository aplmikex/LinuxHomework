#include <QApplication>
#include <QKeySequence>
#include <QMainWindow>

#include "qtermwidget.h"

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);
  QMainWindow *mainWindow = new QMainWindow();
  setenv("TERM", "konsole-256color", 1);
  QTermWidget *console = new QTermWidget(0);

  QFont font = QApplication::font();
  font.setFamily("Monospace");
  font.setPointSize(12);

  console->setTerminalFont(font);

  console->setColorScheme("Ubuntu");
  const QString shell = "/home/xiang/LinuxHomework/build/bin/xsh";
  QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

  mainWindow->setCentralWidget(console);

  mainWindow->show();
  console->setShellProgram(shell);
  console->startShellProgram();

  return app.exec();
}
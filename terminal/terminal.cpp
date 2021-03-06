#include <QApplication>

#include <QDesktopServices>
#include <QKeySequence>
#include <QMainWindow>

#include "qtermwidget.h"


// Here we define the activateUrl method
void activateUrl(const QUrl &url, bool fromContextMenu) {
  if (QApplication::keyboardModifiers() & Qt::ControlModifier ||
      fromContextMenu) {
    QDesktopServices::openUrl(url);
  }
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow *mainWindow = new QMainWindow();
  // setenv("TERM", "konsole-256color", 1);
  mainWindow->resize(700,450);

  QTermWidget *console = new QTermWidget(0);

  QFont font = QApplication::font();
  font.setFamily("Consolas");
  font.setPointSize(11);

  console->setTerminalFont(font);
  console->setColorScheme("Solarized");
  console->setTerminalOpacity(1);
  console->setColorScheme("Ubuntu");
  const QString shell = "./xsh";
  // Here we connect it all together
  QObject::connect(console, &QTermWidget::urlActivated, mainWindow,
                   activateUrl);

  QObject::connect(console, &QTermWidget::termKeyPressed, mainWindow,
                   [=](const QKeyEvent *key) -> void {
                     if (key->matches(QKeySequence::Copy)) {
                       console->copyClipboard();
                     } else if (key->matches(QKeySequence::Paste)) {
                       console->pasteClipboard();
                     }
                   });

  QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));


  mainWindow->setCentralWidget(console);
  console->setShellProgram(shell);
  console->startShellProgram();
  mainWindow->show();
  return app.exec();
}
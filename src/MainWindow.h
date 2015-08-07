#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "LocalLibrary.h"
#include "ExternalLibrary.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
    void openSubWindow(QWidget *window);
    void openExternalSource(ComicsSource *cs);

public:
    explicit MainWindow(QWidget *parent = 0);
    void showMessage(const QString & message, int timeout = 0) {
        statusBar()->showMessage(message, timeout);
    }

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);
private slots:
    void on_actionChoose_Library_triggered();
    void on_actionMangaHere_triggered();
};

#endif // MAINWINDOW_H
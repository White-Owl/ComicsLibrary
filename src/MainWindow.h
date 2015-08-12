#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "LocalLibrary.h"
#include "ExternalLibrary.h"
#include <QProgressBar>

class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
    void openSubWindow(QWidget *window);
    void openExternalSource(ComicsSource *cs);
    QLabel *activeProcess, *eta, *totalJobsInQueue;
    QProgressBar *progressBar;
    quint64 actionStarted;

public:
    explicit MainWindow(QWidget *parent = 0);
    void startAction(QString text);
    void setProgress(quint64 done, quint64 total);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);
private slots:
    void on_actionChoose_Library_triggered();
    void on_actionMangaHere_triggered();
};

#endif // MAINWINDOW_H

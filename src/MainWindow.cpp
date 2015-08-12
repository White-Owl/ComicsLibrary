#include "MainWindow.h"
#include "main.h"
#include "MangaHere.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);

    // restore GUI. Position and size of the main window
    restoreGeometry(settings->value(settingsKey_mainGeometry).toByteArray());
    // restore GUI. Position, size, and visibility of all toolbars
    restoreState(settings->value(settingsKey_mainState).toByteArray());
    // put appropriate checks on the item in the View menu
    actionExternal_Sources->setChecked(externalSourcesToolBar->isVisible());
    actionGeneral_Tools->setChecked(mainToolBar->isVisible());
    actionHistory->setChecked(historyWindow->isVisible());
    actionNetwork_Queue->setChecked(queueWindow->isVisible());

    // Always start with local library window
    localLibraryWindow = new LocalLibrary(this);
    openSubWindow(localLibraryWindow);
    // by default, no external source is opened
    externalLibraryWindow = NULL;


    statusBar_Object->addWidget(activeProcess=new QLabel());
    statusBar_Object->addWidget(progressBar=new QProgressBar());
    statusBar_Object->addWidget(eta=new QLabel());
    statusBar_Object->addWidget(totalJobsInQueue=new QLabel());
    activeProcess->setFixedWidth(300);
    progressBar->setVisible(false);
    progressBar->setTextVisible(false);
    progressBar->setFixedWidth(100);
    eta->setFixedWidth( this->fontMetrics().width("000:00") );
}

void MainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *e) {
    settings->setValue(settingsKey_mainGeometry, saveGeometry());
    settings->setValue(settingsKey_mainState, saveState());
    QWidget::closeEvent(e);
}


void MainWindow::openSubWindow(QWidget *window) {
    QMdiSubWindow *ma = mdiArea->addSubWindow(window);
    ma->setWindowState(Qt::WindowMaximized);
    ma->setFocus();
}


void MainWindow::startAction(QString text) {
    actionStarted = QDateTime::currentMSecsSinceEpoch();
    activeProcess->setText(text);
    progressBar->setMaximum(1);
    progressBar->setValue(0);
    progressBar->setVisible(true);
}

void MainWindow::setProgress(quint64 done, quint64 total) {
    progressBar->setMaximum(total);
    progressBar->setValue(done);
    if(done == 0 && total== 0) {
        eta->setText("");
        return;
    }
    if(done == total) {
        eta->setText("");
        progressBar->setValue(0);
        progressBar->setMaximum(1);
        progressBar->setVisible(false);
        activeProcess->setText("");
        return;
    }

    quint64 elapsed = QDateTime::currentMSecsSinceEpoch() - actionStarted;
    quint64 msecPerUnit = elapsed / done;
    quint64 totalNeeded = msecPerUnit * total;
    quint64 totalLeft = (totalNeeded - elapsed)/1000;
    eta->setText(QString("%1:%2")
                 .arg((quint64)(totalLeft/60), 2, QLatin1Char('0'))
                 .arg((quint64)(totalLeft%60), 2, QLatin1Char('0'))
                 );
}





void MainWindow::on_actionChoose_Library_triggered() {
    if(!localLibraryWindow) {
        localLibraryWindow = new LocalLibrary(this);
        openSubWindow(localLibraryWindow);
    }
    if(localLibraryWindow->chooseLibraryRoot()) {
        localLibraryWindow->readLibrary();
    }
}

void MainWindow::openExternalSource(ComicsSource *cs) {
    if(!externalLibraryWindow) {
        externalLibraryWindow = new ExternalLibrary(this);
        openSubWindow(externalLibraryWindow );
    }
    externalLibraryWindow->setSource(cs);
}

void MainWindow::on_actionMangaHere_triggered() {
    openExternalSource(new MangaHere);
}

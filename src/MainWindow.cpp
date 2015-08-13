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
    progressWindow = new ProgressWindow(this);
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

#include "ProgressWindow.h"
#include "main.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    progressWindow = this;
    setWindowFlags(Qt::Window| Qt::CustomizeWindowHint);
    setFixedSize(size());
}


void ProgressWindow::setCaption(const QString &text) {
    caption->setText(text);
    progressBar->setMaximum(1);
    progressBar->setValue(0);
    show();
    qApp->processEvents(QEventLoop::AllEvents);
}

void ProgressWindow::setProgress(quint64 value, quint64 max) {
    progressBar->setMaximum(max);
    progressBar->setValue(value);
    if(value == max) {
        hide();
        return;
    }
    qApp->processEvents(QEventLoop::AllEvents);
}

void ProgressWindow::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include "ui_ProgressWindow.h"

class ProgressWindow : public QWidget, private Ui::ProgressWindow {
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = 0);
    void setCaption(const QString &text);
    void setProgress(quint64 value, quint64 max=0);

protected:
    void changeEvent(QEvent *e);
};

#endif // PROGRESSWINDOW_H

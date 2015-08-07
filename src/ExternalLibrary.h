#ifndef EXTERNALLIBRARY_H
#define EXTERNALLIBRARY_H

#include "ui_ExternalLibrary.h"
#include "ComicsSource.h"
#include <QStandardItemModel>

class ExternalLibrary : public QWidget, private Ui::ExternalLibrary {
    Q_OBJECT
    ComicsSource *source;
    QString cashedListName;
    QStandardItemModel *titles;

public:
    explicit ExternalLibrary(QWidget *parent = 0);
    void setSource(ComicsSource *source);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

protected slots:
    void finishedListOfTitles(QString error);
};

#endif // EXTERNALLIBRARY_H

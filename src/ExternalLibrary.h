#ifndef EXTERNALLIBRARY_H
#define EXTERNALLIBRARY_H

#include "ui_ExternalLibrary.h"
#include "ComicsSource.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

class ExternalLibrary : public QWidget, private Ui::ExternalLibrary {
    Q_OBJECT
    ComicsSource *source;
    QSortFilterProxyModel *filterModel;
    QStandardItemModel *titles;

    QNetworkAccessManager manager;
    QNetworkReply *reply;


public:
    explicit ExternalLibrary(QWidget *parent = 0);
    void setSource(ComicsSource *source);
    void readTitlesFromCache();
    void writeTitlesToCache();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

protected slots:
    void finishedListOfTitles(QString error);
private slots:
    void on_seriesFilter_textChanged(const QString &text);
    void on_requestCatalogRefresh_clicked();
};

#endif // EXTERNALLIBRARY_H

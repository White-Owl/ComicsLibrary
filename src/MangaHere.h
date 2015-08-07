#ifndef MANGAHERE_H
#define MANGAHERE_H

#include "ComicsSource.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MangaHere : public ComicsSource {
    QString baseURL;
    QNetworkAccessManager manager;
    QNetworkReply *reply;

public:
    MangaHere();
    ~MangaHere();

    void requestListOfTitles();
    void requestComicsInfo(const QString title);
    void requestListOfIssues(const QString title);
    void requestPageCount(const QString title, const QString issue);
    void requestPage(const QString title, const QString issue, uint pageIndex);

private slots:
    void networkError(QNetworkReply::NetworkError errorCode);
    void finishedListOfTitles();
/*    void readyComicsInfo(const QString title, const QString error);
    void readyListOfIssues(const QString title, const QString error);
    void readyPageCount(const QString title, const QString issue, const QString error);
    void readyPage(const QString title, const QString issue, uint pageIndex, QByteArray data, const QString error);*/
};

#endif // MANGAHERE_H

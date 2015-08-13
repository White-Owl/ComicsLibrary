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
    void decryptListOfTitles();
    void decryptComicInfo();
    void decryptListOfIssues();
    void decryptPageCount();
    void decryptPage();
};

#endif // MANGAHERE_H

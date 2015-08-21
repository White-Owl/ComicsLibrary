#ifndef BATOTO_H
#define BATOTO_H

#include "ComicsSource.h"

class Batoto : public ComicsSource {
    QString baseURL;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    uint titlesPerPage;

public:
    Batoto();
    ~Batoto();

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

#endif // BATOTO_H

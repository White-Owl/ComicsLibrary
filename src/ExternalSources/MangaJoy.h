#ifndef MANGAJOY_H
#define MANGAJOY_H

#include "ComicsSource.h"

class MangaJoy : public ComicsSource {
    QString baseURL;
    QNetworkAccessManager manager;
    QNetworkReply *reply;

public:
    MangaJoy();
    ~MangaJoy();

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

#endif // MANGAJOY_H

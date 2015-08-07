#include "MangaHere.h"
#include <QNetworkRequest>
#include <QNetworkReply>

MangaHere::MangaHere() {
    sourceName = "MangaHere";
    baseURL = "http://www.mangahere.com";
}

MangaHere::~MangaHere() {}


void MangaHere::requestListOfTitles() {
    comicsData.clear();
    reply = manager.get(QNetworkRequest(QUrl(QString("%1/mangalist/").arg(baseURL))));
    connect(reply, SIGNAL(finished()), this, SLOT(finishedListOfTitles()));
}

void MangaHere::finishedListOfTitles() {
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError) {
        emit readyListOfTitles(reply->errorString());
        return;
    }

    // Example Entry: <li><a class="manga_info" rel="&quot;Gokko&quot;" href="http://www.mangahere.com/manga/gokko/"><span></span>&quot;Gokko&quot;</a></li>
    QRegExp re("<li><a class=\"manga_info\" rel=\"[^\"]*\" href=\"([^\"]*)\"><span></span>([^\"]*)</a></li>");
    QByteArray contents = reply->readAll();
    int pos = 0;
    while ((pos = re.indexIn(contents, pos)) != -1) {
        Comics comics;
        comics.title = re.cap(2);
        comics.url = re.cap(1);
        comicsData[comics.title] = comics;
        pos += re.matchedLength();
    }
    emit readyListOfTitles("");
}


void MangaHere::requestComicsInfo(const QString title) {
    Q_UNUSED(title);
}
void MangaHere::requestListOfIssues(const QString title) {
    Q_UNUSED(title);
}
void MangaHere::requestPageCount(const QString title, const QString issue) {
    Q_UNUSED(title);
    Q_UNUSED(issue);
}
void MangaHere::requestPage(const QString title, const QString issue, uint pageIndex) {
    Q_UNUSED(title);
    Q_UNUSED(issue);
    Q_UNUSED(pageIndex);
}

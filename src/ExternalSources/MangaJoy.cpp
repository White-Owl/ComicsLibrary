#include "main.h"
#include "MangaJoy.h"
#include <QNetworkRequest>
#include <QNetworkReply>

MangaJoy::MangaJoy() {
    sourceName = "MangaJoy";
    baseURL = "http://manga-joy.com";
}

MangaJoy::~MangaJoy() {}


void MangaJoy::requestListOfTitles() {
    comicsData.clear();
    reply = manager.get(QNetworkRequest(QUrl(QString("%1/manga-list-all/").arg(baseURL))));
    connect(reply, SIGNAL(finished()), this, SLOT(decryptListOfTitles()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress1stStep(qint64,qint64)));
}

void MangaJoy::decryptListOfTitles() {
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError) {
        emit readyListOfTitles(reply->errorString());
        return;
    }

    // Example Entry: <li><a href="http://manga-joy.com/B-Gata-H-Kei/" title="B Gata H Kei" rel="678" class="mng_det_pop">B Gata H Kei</a> </li>
    QRegExp re("<li><a href=\"([^\"]*)\"[^>]*>([^<]*)</a> *</li>");
    QByteArray contents = reply->readAll();

    qDebug() << "bytes in the reply" << contents.size();
    QFile tmp("./contents.tmp");
    tmp.open(QIODevice::WriteOnly);
    tmp.write(contents);
    tmp.close();

    progressWindow->setCaption(tr("Decypher list of titles"));
    int pos = 0;
    int comicsCount = 0;
    while ((pos = re.indexIn(contents, pos)) != -1) {
        Comics comics;
        comics.title = decodeHTML( re.cap(2) );
        comics.url = re.cap(1);
        comicsData[comics.title] = comics;
        pos += re.matchedLength();
        if(comicsCount % 500 == 0) {
            progressWindow->setProgress(pos, contents.length());
        }
        comicsCount++;
    }
    progressWindow->hide();
    emit readyListOfTitles("");
}


void MangaJoy::requestComicsInfo(const QString title) {
    Q_UNUSED(title);
}

void MangaJoy::decryptComicInfo() {
    emit readyComicsInfo("", "");
}

void MangaJoy::requestListOfIssues(const QString title) {
    Q_UNUSED(title);
}

void MangaJoy::decryptListOfIssues() {
    emit readyListOfIssues("", "");
}

void MangaJoy::requestPageCount(const QString title, const QString issue) {
    Q_UNUSED(title);
    Q_UNUSED(issue);
}

void MangaJoy::decryptPageCount() {
    emit readyPageCount("", "", "");
}

void MangaJoy::requestPage(const QString title, const QString issue, uint pageIndex) {
    Q_UNUSED(title);
    Q_UNUSED(issue);
    Q_UNUSED(pageIndex);
}
void MangaJoy::decryptPage() {
    emit readyPage("", "", 0, QByteArray(), "");
}

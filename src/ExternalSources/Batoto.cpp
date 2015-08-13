#include "main.h"
#include "Batoto.h"

Batoto::Batoto() {
    sourceName = "Batoto";
    baseURL = "http://www.bato.to";
}

Batoto::~Batoto() {}

void Batoto::requestListOfTitles() {
    comicsData.clear();
    reply = manager.get(QNetworkRequest(QUrl(QString("%1/mangalist/").arg(baseURL))));
    connect(reply, SIGNAL(finished()), this, SLOT(decryptListOfTitles()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress1stStep(qint64,qint64)));
}

void Batoto::decryptListOfTitles() {
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError) {
        emit readyListOfTitles(reply->errorString());
        return;
    }

    // Example Entry: <li><a class="manga_info" rel="&quot;Gokko&quot;" href="http://www.Batoto.com/manga/gokko/"><span></span>&quot;Gokko&quot;</a></li>
    QRegExp re("<li><a class=\"manga_info\" rel=\"[^\"]*\" href=\"([^\"]*)\"><span></span>([^\"]*)</a></li>");
    QByteArray contents = reply->readAll();
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


void Batoto::requestComicsInfo(const QString title) {
    Q_UNUSED(title);
}

void Batoto::decryptComicInfo() {
    emit readyComicsInfo("", "");
}

void Batoto::requestListOfIssues(const QString title) {
    Q_UNUSED(title);
}

void Batoto::decryptListOfIssues() {
    emit readyListOfIssues("", "");
}

void Batoto::requestPageCount(const QString title, const QString issue) {
    Q_UNUSED(title);
    Q_UNUSED(issue);
}

void Batoto::decryptPageCount() {
    emit readyPageCount("", "", "");
}

void Batoto::requestPage(const QString title, const QString issue, uint pageIndex) {
    Q_UNUSED(title);
    Q_UNUSED(issue);
    Q_UNUSED(pageIndex);
}
void Batoto::decryptPage() {
    emit readyPage("", "", 0, QByteArray(), "");
}


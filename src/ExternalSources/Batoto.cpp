#include "main.h"
#include "Batoto.h"

Batoto::Batoto() {
    sourceName = "Batoto";
    baseURL = "http://bato.to";
    titlesPerPage = 500;
}

Batoto::~Batoto() {}

void Batoto::requestListOfTitles() {
    comicsData.clear();
    reply = manager.get(QNetworkRequest(QUrl(QString("%1/comic/_/comics/?per_page=%2&st=0").arg(baseURL).arg(titlesPerPage))));
    connect(reply, SIGNAL(finished()), this, SLOT(decryptListOfTitles()));
}


void Batoto::decryptListOfTitles() {
    //qDebug() << reply->request().url();
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError) {
        emit readyListOfTitles(reply->errorString());
        return;
    }

    QByteArray contents = reply->readAll();

    int pos = 0;
    /// searching for number of pages
    /// <a href='#'>Page 1 of 15 <!.....
    QRegExp rePages("<a href=[\"\']#[\"\']>Page (\\d+) of (\\d+) <");
    if((pos=rePages.indexIn(contents)) == -1) {
        emit readyListOfTitles(tr("Failed to read number of pages"));
        return;
    }
    uint currentPageInTheListOfTitles = rePages.cap(1).toUInt();
    uint totalPagesInTheListOfTitles = rePages.cap(2).toUInt();
    if(currentPageInTheListOfTitles == 1 ) {
        progressWindow->setCaption(tr("Decypher list of titles"));
    }
    progressWindow->setProgress(currentPageInTheListOfTitles-1, totalPagesInTheListOfTitles);



    /// Example Entry:
    /// Start of record: <tr class="__topic ">
    /// then several lines of formatting and finaly the title:
    /// <a href="http://bato.to/comic/_/comics/choko-beast-r11065">Choko Beast!!</a>
    QRegExp reRecordStart("<tr class=[\"\']__topic *[\"\']>");
    QRegExp reTitle("<a href=[\"']([^\"']+)[\"']>([^<]+)</a>");
    while ((pos = reRecordStart.indexIn(contents, pos)) != -1) {
        Comics comics;

        pos += reRecordStart.matchedLength();
        pos = reTitle.indexIn(contents, pos);
        if(pos == -1) break; // it should not happen, but just in case
        pos += reTitle.matchedLength();

        comics.title = decodeHTML(reTitle.cap(2));
        comics.url = reTitle.cap(1);
        comicsData[comics.title] = comics;
        //qDebug() << "Title:" << comics.title << "Url:" << comics.url;
    }


    if(currentPageInTheListOfTitles < totalPagesInTheListOfTitles) {
        reply = manager.get(QNetworkRequest(QUrl(QString("%1/comic/_/comics/?per_page=%2&st=%3")
                                                 .arg(baseURL)
                                                 .arg(titlesPerPage)
                                                 .arg(currentPageInTheListOfTitles*titlesPerPage)
                                                 )));
        connect(reply, SIGNAL(finished()), this, SLOT(decryptListOfTitles()));
    } else {
        progressWindow->hide();
        emit readyListOfTitles("");
    }
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

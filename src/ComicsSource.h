#ifndef COMICSOURCE_H
#define COMICSOURCE_H

#include <QtCore>

struct Comics {
    // Title for the comics
    QString title;
    // Where this comics starting page?
    QString url;
    // Short (or long) description
    QString summary;
    // Image to represent the comics
    QByteArray cover;
    // list of genres this comics belong to
    QList<QString> genres;
    // Is the comics completed or still in production?
    bool completed;
    // Is it available for download or was it blocked (ie by licensing reason).
    bool available;

    // List of urls and page counts by issue/volume title
    QHash<QString, QPair<QString, int> > issues;
};


class ComicsSource : public QObject {
    Q_OBJECT

public:
    // This would be used to show in the Source in GUI
    QString sourceName;

    // This will hold the information about comicses available at the source.
    // Warning! Do not fill the storage in one run. Fill it up on a per-need basis.
    QHash<QString, Comics> comicsData;


    // Fill up the hash with all comics known to the source.
    // Only title and url fields are expected to be completed.
    // All other fields can be left empty.
    virtual void requestListOfTitles() = 0;


    // Read the detailed description of an individual comic and store in the comicsData
    virtual void requestComicsInfo(const QString title) = 0;

    // Fills up issues field
    virtual void requestListOfIssues(const QString title) = 0;

    // Request page count for the individual issue.
    virtual void requestPageCount(const QString title, const QString issue) = 0;

    // Request the specified page from a particular comics/issue
    virtual void requestPage(const QString title, const QString issue, uint pageIndex) = 0;

signals:
    void readyListOfTitles(const QString error);
    void readyComicsInfo(const QString title, const QString error);
    void readyListOfIssues(const QString title, const QString error);
    void readyPageCount(const QString title, const QString issue, const QString error);
    void readyPage(const QString title, const QString issue, uint pageIndex, QByteArray data, const QString error);
};

#endif // COMICSOURCE_H


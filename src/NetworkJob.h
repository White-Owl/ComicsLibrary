#ifndef NETWORKJOB_H
#define NETWORKJOB_H

#include <QtCore>

struct NetworkJob {
public:
    QString title;
    QString chapter;
    QString page;

    NetworkJob(const QString &title,
               const QString &chapter = QString(), const QString &page = QString()) {
        this->title = title;
        this->chapter = chapter;
        this->page = page;
    }
};

#endif // NETWORKJOB_H

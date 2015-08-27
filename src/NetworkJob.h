#ifndef NETWORKJOB_H
#define NETWORKJOB_H

#include "ComicsSource.h"

class NetworkJob {
    Q_OBJECT

    ComicsSource *source;
    QString title;
    QString chapter;
    QString page;
public:
    NetworkJob(const ComicsSource *source, const QString &title, const QString &chapter, const QString &page);

    void start();
    void pause();
    void cancel();

signals:
    void finished(QString error);
};

#endif // NETWORKJOB_H
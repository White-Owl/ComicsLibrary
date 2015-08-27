#include "ComicsSource.h"

void ComicsSource::loadCachedListOfTitles() {
    QStringList cacheDirs;
#if QT_VERSION >= 0x050000
    cacheDirs << QStandardPaths::standardLocations(QStandardPaths::CacheLocation);
    if(cacheDirs.isEmpty()) cacheDirs << QDir::tempPath();
#else
    cacheDirs << QString("~/.cache/%1").arg(qApp->applicationName());
#endif
    if(! QFileInfo::exists(cacheDirs[0])) QDir::root().mkpath(cacheDirs[0]);

    cachedTitlesFileName = QString("%1/%2.dat").arg(cacheDirs[0]).arg(sourceName);
    QFile cachedListFile(cachedTitlesFileName);
    cachedListFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!cachedListFile.isOpen()) return;

    while(!cachedListFile.atEnd()) {
        QList<QByteArray> line = cachedListFile.readLine().split('\t');
        if(line.size()<2) continue; // should not happen, but if someone "edited" the file...

        Comics comics;
        comics.title = line[0];
        comics.url = line[1];
        comicsData[comics.title] = comics;
    }
    cachedListFile.close();

}

void ComicsSource::saveListOfTitlesToCache() {
    QFile cachedListFile(cachedTitlesFileName );
    cachedListFile.open(QIODevice::WriteOnly);
    if(!cachedListFile.isOpen()) return;

    QTextStream fileStream(&cachedListFile);
    QHashIterator<QString, Comics> itr(comicsData);
    while (itr.hasNext()) {
        itr.next();
        fileStream << itr.value().title << '\t' << itr.value().url << endl;
    }
    fileStream.flush();
    cachedListFile.close();
}

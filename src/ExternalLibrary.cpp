#include "ExternalLibrary.h"
#include "main.h"

ExternalLibrary::ExternalLibrary(QWidget *parent) : QWidget(parent) {
    setupUi(this);

    titles = new QStandardItemModel(this);
    filterModel = new QSortFilterProxyModel(this);
//    filterModel->setSourceModel(titles);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    listComics->setModel(filterModel);
}

void ExternalLibrary::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void ExternalLibrary::closeEvent(QCloseEvent *e) {
    externalLibraryWindow = NULL;
    QWidget::closeEvent(e);
}

void ExternalLibrary::setSource(ComicsSource *source) {
    this->source = source;
    setWindowTitle(source->sourceName);
    this->setEnabled(false);

    QFileInfo fi(settings->fileName());
    cashedListName = QString("%1/%2.dat").arg(fi.path()).arg(source->sourceName);
    QFile cashedListFile(cashedListName);
    if(cashedListFile.exists()) {
        progressWindow->setCaption(QString(tr("Reading cashed list of titles for %1"))
                                   .arg(source->sourceName));
        QFileInfo fi(cashedListFile);
        quint64 fileSize = fi.size();
        cashedListFile.open(QIODevice::ReadOnly | QIODevice::Text);
        while(!cashedListFile.atEnd()) {
            QList<QByteArray> line = cashedListFile.readLine().split('\t');
            if(line.size()<2) continue; // should not happen, but if someone "edited" the file...

            Comics comics;
            comics.title = line[0];
            comics.url = line[1];
            source->comicsData[comics.title] = comics;
            progressWindow->setProgress(cashedListFile.pos(), fileSize);
        }
        cashedListFile.close();
        //qDebug() << source->comicsData.keys();
        progressWindow->setProgress(fileSize, fileSize);
        finishedListOfTitles("");
    } else {
        on_requestCatalogRefresh_clicked();
    }
    qDebug() << cashedListFile.fileName();
}

void ExternalLibrary::downloadProgress(qint64 done, qint64 total) {
    progressWindow->setProgress(done, total);
}

void ExternalLibrary::on_requestCatalogRefresh_clicked() {
    progressWindow->setCaption(QString(tr("Reading list of titles for %1"))
                               .arg(source->sourceName));
    connect(source, SIGNAL(readyListOfTitles(QString)), this, SLOT(finishedListOfTitles(QString)));
    connect(source, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    QFile cashedListFile(cashedListName);
    cashedListFile.remove();
    source->requestListOfTitles();
}

void ExternalLibrary::finishedListOfTitles(QString error){
    if(!error.isEmpty()) {
        qDebug() << "finishedListOfTitles error:" << error;
        this->setEnabled(true);
        progressWindow->hide();
        QMessageBox::warning(this, tr("Failed to read list of titles"), error);
        return;
    }

    QFileInfo cashedListFI(cashedListName);
    if(!cashedListFI.exists()) {
        progressWindow->setCaption(QString(tr("Saving list of titles for %1"))
                                   .arg(source->sourceName));
        quint64 totalTitles = source->comicsData.keys().count();
        qDebug() << "saving" << totalTitles << "titles into" << cashedListName;
        quint64 titlesCached = 0;
        QFile cashedListFile(cashedListName);
        cashedListFile.open(QIODevice::WriteOnly);
        QTextStream cashedList(&cashedListFile);
        QHashIterator<QString, Comics> itr(source->comicsData);
        while (itr.hasNext()) {
            itr.next();
            cashedList << itr.key() << '\t' << itr.value().url << endl;
            titlesCached++;
            if(titlesCached%1000 == 0) {
                progressWindow->setProgress(titlesCached, totalTitles);
                qDebug() << "saved" << titlesCached<< "of" << totalTitles;
            }
        }
        cashedList.flush();
        catalogRefreshedAt->setText(QDateTime::currentDateTime().toLocalTime().toString());
        cashedListFile.close();
        qDebug() << "Saving done";
    } else {
        catalogRefreshedAt->setText(cashedListFI.created().toLocalTime().toString());
    }

    progressWindow->setCaption(tr("Refreshing"));
    filterModel->setSourceModel(NULL);
    titles->clear();
    quint64 titlesCount = source->comicsData.keys().count();
    QHashIterator<QString, Comics> itr(source->comicsData);
    while (itr.hasNext()) {
        itr.next();
        QStandardItem *item = new QStandardItem();
        item->setText(itr.value().title);
        titles->appendRow(item);
        if(titles->rowCount() % 1000 == 0) {
            progressWindow->setProgress(titles->rowCount(), titlesCount);
        }
    }
    progressWindow->hide();
    filterModel->setSourceModel(titles);

    this->setEnabled(true);
    filterModel->sort(0);
}

void ExternalLibrary::on_seriesFilter_textChanged(const QString &text) {
    filterModel->setFilterWildcard(text);
    filterModel->sort(0);
}


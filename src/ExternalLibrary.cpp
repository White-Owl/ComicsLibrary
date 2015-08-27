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
    finishedListOfTitles();
}


void ExternalLibrary::on_requestCatalogRefresh_clicked() {
    progressWindow->setCaption(QString(tr("Requesting list of titles for %1"))
                               .arg(source->sourceName));
    connect(source, SIGNAL(readyListOfTitles(QString)), this, SLOT(finishedListOfTitles(QString)));
    QFile cashedListFile(cachedListName);
    cashedListFile.remove();
    source->requestListOfTitles();
}

void ExternalLibrary::finishedListOfTitles(QString error){
    if(!error.isEmpty()) {
        this->setEnabled(true);
        progressWindow->hide();
        QMessageBox::warning(this, tr("Failed to read list of titles"), error);
        return;
    }

    QFileInfo cashedListFI(cachedListName);
    if(!cashedListFI.exists()) {
        progressWindow->setCaption(QString(tr("Saving list of titles for %1"))
                                   .arg(source->sourceName));
        quint64 totalTitles = source->comicsData.keys().count();
        quint64 titlesCached = 0;
        QFile cashedListFile(cachedListName);
        cashedListFile.open(QIODevice::WriteOnly);
        QTextStream cashedList(&cashedListFile);
        QHashIterator<QString, Comics> itr(source->comicsData);
        while (itr.hasNext()) {
            itr.next();
            cashedList << itr.key() << '\t' << itr.value().url << endl;
            titlesCached++;
            if(titlesCached%1000 == 0) {
                progressWindow->setProgress(titlesCached, totalTitles);
            }
        }
        cashedList.flush();
        catalogRefreshedAt->setText(QDateTime::currentDateTime().toLocalTime().toString());
        cashedListFile.close();
    } else {
        catalogRefreshedAt->setText(cashedListFI.created().toLocalTime().toString());
    }

    quint64 titlesCount = source->comicsData.keys().count();
    filterModel->setSourceModel(NULL);
    titles->clear();
    if(titlesCount==0) {
        QMessageBox::warning(this, tr("Failed to read list of titles"),
                             tr("There were no network errors but the title list is empty.\n"
                                "Most likely there was a change of data structure on the source web-site.\n"
                                "Please inform developers about this problem.")
                             );
    } else {
        progressWindow->setCaption(tr("Refreshing"));
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


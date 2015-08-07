#include "ExternalLibrary.h"
#include "main.h"

ExternalLibrary::ExternalLibrary(QWidget *parent) : QWidget(parent) {
    setupUi(this);
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
    connect(source, SIGNAL(readyListOfTitles(QString)), this, SLOT(finishedListOfTitles(QString)));
    QFileInfo fi(settings->fileName());
    cashedListName = QString("%1/%2.dat").arg(fi.path()).arg(source->sourceName);
    QFile cashedList(cashedListName);
    if(cashedList.exists()) {
        mainWindow->showMessage(
                    QString(tr("Reading cashed list of titles for %1"))
                    .arg(source->sourceName));

        cashedList.open(QIODevice::ReadOnly | QIODevice::Text);
        while(!cashedList.atEnd()) {
            QList<QByteArray> line = cashedList.readLine().split('\t');
            Comics comics;
            comics.title = line[0];
            comics.url = line[1];
            source->comicsData[comics.title] = comics;
        }
        cashedList.close();
        qDebug() << source->comicsData.keys();
        emit source->readyListOfTitles("");
    } else {
    //    source->requestListOfTitles();
    }
    this->setEnabled(false);
    qDebug() << cashedList.fileName();
}

void ExternalLibrary::finishedListOfTitles(QString error){
    if(!error.isEmpty()) {
        mainWindow->showMessage(error);
        return;
    }

    QFileInfo cashedListFI(cashedListName);
    if(!cashedListFI.exists()) {
        QFile cashedListFile(cashedListName);
        QTextStream cashedList(&cashedListFile);
        QHashIterator<QString, Comics> itr(source->comicsData);
        while (itr.hasNext()) {
            itr.next();
            cashedList << itr.key() << '\t' << itr.value().url << endl;
        }
        cashedList.flush();
        catalogRefreshedAt->setText(QDateTime::currentDateTime().toLocalTime().toString());
    } else {
        catalogRefreshedAt->setText(cashedListFI.created().toLocalTime().toString());
    }



    QHashIterator<QString, Comics> itr(source->comicsData);
    while (itr.hasNext()) {
        itr.next();
        cashedList << itr.key() << '\t' << itr.value().url << endl;
    }


    this->setEnabled(true);
    mainWindow->showMessage("Ok", 500);

}

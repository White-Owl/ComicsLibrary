#include "LocalLibrary.h"
#include "main.h"

static const int iconSizes[] = {0, 26, 32, 48, 64, 128, 256, 512};
static const char descriptionFileName[] = "description.txt";

LocalLibrary::LocalLibrary(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    setWindowTitle(tr("Local Library"));
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
    qDebug() << "LocalLibrary()";
    libraryPath = settings->value(settingsKey_libraryPath).toString();
    libraryData = new QStandardItemModel(this);
    filterLibrary = new QSortFilterProxyModel(this);
    libraryView->setModel(filterLibrary);
    filterLibrary->setFilterCaseSensitivity(Qt::CaseInsensitive);

    iconSizeSelector->setValue(settings->value(settingsKey_iconSize, 0).toInt());
    connect(libraryView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(libraryView_selectionChanged(QModelIndex,QModelIndex)));

    for(uint i=1; i<sizeof(iconSizes)/sizeof(iconSizes[0]); i++ ) {
        QString iconName = QString(":Images/magazine_%1.png").arg(iconSizes[i]);
        defaultComicIcon.addFile(iconName);
    }

    comicsSource->addItems(comicsSources.keys());

    comicsSourceModel = qobject_cast<QStandardItemModel*>(comicsSource->model());
    comicsSourceModel->sort(0);
}

void LocalLibrary::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void LocalLibrary::closeEvent(QCloseEvent *e) {
    localLibraryWindow = NULL;
    QWidget::closeEvent(e);
}

void LocalLibrary::on_iconSizeSelector_valueChanged(int value) {
    if((value < 0) || (uint)value > sizeof(iconSizes)/sizeof(iconSizes[0])) {
        value=0;
    }
    int size=iconSizes[value];
    if(size==0) {
        libraryView->setViewMode(QListView::ListMode);
        libraryView->setIconSize(QSize());
    } else {
        libraryView->setViewMode(QListView::IconMode);
        libraryView->setIconSize(QSize(size,size));
    }
    settings->setValue(settingsKey_iconSize, value);
}

bool LocalLibrary::chooseLibraryRoot() {
    QString path = QFileDialog::getExistingDirectory(this,
                        tr("Please choose a folder for the library"),
                        settings->value(settingsKey_libraryPath, QDir::homePath()).toString(),
                        QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return false;
    settings->setValue(settingsKey_libraryPath, path );
    libraryPath = settings->value(settingsKey_libraryPath).toString();
    return true;
}

bool LocalLibrary::readLibrary() {
    libraryData->clear();
    if(libraryPath.isEmpty() && !chooseLibraryRoot()) return false;

    progressWindow->setCaption(tr("Reading local library"));
    filterLibrary->setSourceModel(NULL);
    QDir libraryDir(libraryPath);
    QStringList folders = libraryDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString folderName, folders) {
        QStandardItem *item = new QStandardItem();
        item->setText(folderName);
        QDir folder(QString("%1/%2").arg(libraryPath).arg(folderName));
        QStringList covers = folder.entryList(QStringList()<<"cover.png"<<"cover.jpg"<<"cover.gif", QDir::NoFilter, QDir::Name);
        if(covers.count()>0) {
            QPixmap coverImage(QPixmap(QString("%1/%2/%3").arg(libraryPath).arg(folderName).arg(covers[0])));
            QIcon coverIcon;
            for(uint i=1; i<sizeof(iconSizes)/sizeof(iconSizes[0]); i++ ) {
                coverIcon.addPixmap(coverImage.scaled(iconSizes[i], iconSizes[i], Qt::KeepAspectRatio));
            }
            item->setIcon(coverIcon);
        } else {
            item->setIcon(defaultComicIcon);
        }
        libraryData->appendRow(item);
        progressWindow->setProgress(libraryData->rowCount(), folders.count());
    }
    filterLibrary->setSourceModel(libraryData);
    progressWindow->hide();
    return true;
}

void LocalLibrary::updateComicDescription(const QModelIndex &index) {
    if(index.isValid()) {
        QModelIndex dataIndex = filterLibrary->mapToSource(index);
        QVariant item = libraryData->data( dataIndex );
        qDebug() << "describe" << item;

        QString folderName = libraryData->data( dataIndex ).toString();
        comicsTitle->setText(folderName);
        QDir folder(QString("%1/%2").arg(libraryPath).arg(folderName));
        QStringList covers = folder.entryList(QStringList()<<"cover.png"<<"cover.jpg"<<"cover.gif", QDir::NoFilter, QDir::Name);
        if(covers.count()>0) {
            QPixmap coverImage(QPixmap(QString("%1/%2/%3").arg(libraryPath).arg(folderName).arg(covers[0])));
            comicsCover->setPixmap(coverImage.scaled(QSize(256,256), Qt::KeepAspectRatio));
            //comicCover->setPixmap(coverImage);
        } else {
            comicsCover->setPixmap(QPixmap(":/Images/magazine_256.png"));
        }

        QStringList folderContents = folder.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::Name);
        QRegExp possibleChapterNames("^[^.]*$|.*\\.cbz");
        QStringList chapters = folderContents.filter(possibleChapterNames);
        comicsTotalIssues->setText(QString("%1").arg(chapters.count()));
        QDateTime lastRefresh(QDate(1900,1,1), QTime(0,0,0));
        foreach(QString chapterName, chapters) {
            QFileInfo fi(QString("%1/%2").arg(folder.absolutePath()).arg(chapterName));
            if(fi.lastModified()>lastRefresh) {
                lastRefresh = fi.lastModified();
            }
        }
        comicLastUpdated->setText(lastRefresh.toString());

        comicsSummary->setText(comicsDescription[cSettingsKey_summary].toString());

        bool canUpdate = false;
        for(int i=0; i<comicsSourceModel->rowCount(); i++) {
            QStandardItem *item = comicsSourceModel->item(i);
            ComicsSource *cs = comicsSources[item->text()];
            if(cs->comicsData.contains(folderName)) {
                canUpdate = true;
                item->setEnabled(true);
            } else {
                item->setEnabled(false);
            }
        }
        comicsSource->setEnabled(true);
        if (canUpdate) {
            comicsSource->setCurrentIndex( comicsSource->findText(comicsDescription[cSettingsKey_source].toString()) );
        } else {
            comicsSource->setCurrentIndex( -1);
        }
        if(comicsSource->currentIndex()<0) {
            comicsUpdatable->setEnabled(false);
            comicsUpdatable->setChecked(false);
        } else {
            comicsUpdatable->setEnabled(true);
            comicsUpdatable->setChecked(comicsDescription[cSettingsKey_updatable].toBool());
        }

    } else {
        comicsTitle->setText(QString(tr("Local Library")));
        comicsCover->setPixmap(QPixmap(":/Images/magazine_256.png"));
        comicsSummary->setText(libraryPath);
        comicsSource->setEnabled(false);
        QFileInfo fi(libraryPath);
        comicLastUpdated->setText(fi.lastModified().toLocalTime().toString());
        comicsTotalIssues->setText(QString("%1").arg(libraryData->rowCount()));
        comicsUpdatable->setEnabled(false);
        comicsUpdatable->setChecked(false);
    }
}

QString LocalLibrary::infoFileName(const QString &title) {
    QString infoFile = QString("%1/%2/%3").arg(libraryPath)
            .arg(title)
            .arg(descriptionFileName);
    return infoFile;
}

void LocalLibrary::libraryView_selectionChanged(const QModelIndex &selected, const QModelIndex &deselected) {
    if(deselected.isValid()) {
        QString fileName = infoFileName(libraryData->data( filterLibrary->mapToSource(deselected)).toString());
        saveHash(fileName, comicsDescription);
    }
    if(selected.isValid()) {
        QString fileName = infoFileName(libraryData->data( filterLibrary->mapToSource(selected)).toString());
        readHash(fileName, comicsDescription);
    }
    updateComicDescription(selected);
}

void LocalLibrary::on_seriesFilter_textChanged(const QString &text) {
    filterLibrary->setFilterWildcard(text);
}


void LocalLibrary::on_comicsUpdatable_toggled(bool checked) {
    qDebug() << "on_comicsDisableUpdates_toggled" << checked;
    comicsDescription[cSettingsKey_updatable] = checked;
}

void LocalLibrary::on_comicsSource_currentIndexChanged(const QString &text) {
    qDebug() << "on_comicsSource_currentIndexChanged(" << text << ")";
    qDebug() << libraryView->currentIndex() << libraryView->currentIndex().isValid();
    if(!libraryView->currentIndex().isValid()) {
        comicsSource->setCurrentIndex(-1);
        comicsSource->setEnabled(false);
        comicsUpdatable->setChecked(false);
        comicsUpdatable->setEnabled(false);
        return;
    }
    comicsDescription[cSettingsKey_source] = text;
    comicsUpdatable->setEnabled(true);
}


void LocalLibrary::checkAllForUpdates() {
    QDir libraryDir(libraryPath);
    QStringList folders = libraryDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString folderName, folders) {
        checkForUpdates(folderName);
    }
}

void LocalLibrary::checkForUpdates(const QString &title) {
    QHash<QString, QVariant> description;
    readHash(infoFileName(title), description);
    if((!description[cSettingsKey_source].toString().isEmpty()) && description[cSettingsKey_updatable].toBool()) {
        qDebug() << "Update" << title;
        comicsSources[description[cSettingsKey_source].toString()]->networkQueue.enqueue(
                    NetworkJob(description[cSettingsKey_source].toString(), title));
    }
}

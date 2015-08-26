#include "LocalLibrary.h"
#include "main.h"

static const int iconSizes[] = {0, 26, 32, 48, 64, 128, 256, 512};
static const char descriptionFileName[] = "description.txt";

LocalLibrary::LocalLibrary(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    setWindowTitle(tr("Local Library"));
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);

    libraryPath = settings->value(settingsKey_libraryPath).toString();
    libraryData = new QStandardItemModel(this);
    filterLibrary = new QSortFilterProxyModel(this);
    filterLibrary->setSourceModel(libraryData);
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
    progressWindow->setProgress(0);
    return true;
}

void LocalLibrary::updateComicDescription(const QModelIndex &index) {
    if(index.isValid() && index.model()==filterLibrary) {
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


        QString infoFile = QString("%1/%2/%3").arg(libraryPath).arg(folderName).arg(descriptionFileName);
        QSettings comicsInfo(infoFile, QSettings::IniFormat);
        comicsSummary->setText(comicsInfo.value(cSettingsKey_summary).toString());


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
            comicsSource->setCurrentIndex( comicsSource->findText(comicsInfo.value(cSettingsKey_source).toString()) );
        } else {
            comicsSource->setCurrentIndex( -1);
        }
        if(comicsSource->currentIndex()<0) {
            comicsDisableUpdates->setEnabled(false);
            comicsDisableUpdates->setChecked(true);
        } else {
            comicsDisableUpdates->setEnabled(true);
            comicsDisableUpdates->setChecked(comicsInfo.value(cSettingsKey_updatable, true).toBool());
        }

    } else {
        comicsTitle->setText(QString(tr("Local Library")));
        comicsCover->setPixmap(QPixmap(":/Images/magazine_256.png"));
        comicsSummary->setText(libraryPath);
        comicsSource->setEnabled(false);
        QFileInfo fi(libraryPath);
        comicLastUpdated->setText(fi.lastModified().toLocalTime().toString());
        comicsTotalIssues->setText(QString("%1").arg(libraryData->rowCount()));
        comicsDisableUpdates->setEnabled(false);
        comicsDisableUpdates->setChecked(false);
    }
}

void LocalLibrary::on_libraryView_activated(const QModelIndex &index) {
    QVariant item = libraryData->data( filterLibrary->mapToSource(index));
    qDebug() << "read" << item;
}

void LocalLibrary::libraryView_selectionChanged(const QModelIndex &selected, const QModelIndex &deselected) {
    Q_UNUSED(deselected);
    updateComicDescription(selected);
}


void LocalLibrary::on_comicsDisableUpdates_toggled(bool checked) {
    QString infoFile = QString("%1/%2/%3").arg(libraryPath)
            .arg(libraryData->data(filterLibrary->mapToSource(libraryView->currentIndex())).toString())
            .arg(descriptionFileName);
    QSettings comicInfo(infoFile, QSettings::IniFormat);
    comicInfo.setValue(cSettingsKey_updatable, checked);
}

void LocalLibrary::on_comicsSource_currentIndexChanged(const QString &text) {
    QString infoFile = QString("%1/%2/%3").arg(libraryPath)
            .arg(libraryData->data(filterLibrary->mapToSource(libraryView->currentIndex())).toString())
            .arg(descriptionFileName);
    QSettings comicInfo(infoFile, QSettings::IniFormat);
    comicInfo.setValue(cSettingsKey_source, text);
}

void LocalLibrary::on_seriesFilter_textChanged(const QString &text) {
    filterLibrary->setFilterWildcard(text);
}

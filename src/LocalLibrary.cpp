#include "LocalLibrary.h"
#include "main.h"

static const int iconSizes[] = {0, 26, 32, 48, 64, 128, 256, 512};

LocalLibrary::LocalLibrary(QWidget *parent) : QWidget(parent) {
    setupUi(this);
    setWindowTitle(tr("Local Library"));
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);

    libraryPath = settings->value(settingsKey_libraryPath).toString();
    libraryData = new QStandardItemModel(this);
    libraryView->setModel(libraryData);
    iconSizeSelector->setValue(settings->value(settingsKey_iconSize, 0).toInt());
    connect(libraryView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(libraryView_selectionChanged(QModelIndex,QModelIndex)));

    for(uint i=1; i<sizeof(iconSizes)/sizeof(iconSizes[0]); i++ ) {
        QString iconName = QString(":Images/magazine_%1.png").arg(iconSizes[i]);
        defaultComicIcon.addFile(iconName);
    }
    quickTabs->setShape(QTabBar::RoundedWest);
    //quickTabs->setAutoHide(true);
    quickTabs->setExpanding(true);
    quickTabs->setDocumentMode(true);
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
    QDir libraryDir(libraryPath);
    QChar prevAccessBy=' ';
    uint comicsesFound = 0;
    foreach(QString folderName, libraryDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
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

        QChar accessBy = folderName.at(0).toUpper();
        if(accessBy<'A') accessBy = '#';
        if(accessBy != prevAccessBy) {
            int tabIndex = quickTabs->addTab(accessBy);
            quickTabs->setTabData(tabIndex, comicsesFound);
            prevAccessBy = accessBy;
        }
        comicsesFound ++;
    }
    updateComicDescription(QModelIndex());
    return true;
}

void LocalLibrary::updateComicDescription(const QModelIndex &index) {
    if(index.isValid()) {
        QString folderName = libraryData->data( index ).toString();
        comicTitle->setText(folderName);
        QDir folder(QString("%1/%2").arg(libraryPath).arg(folderName));
        QStringList covers = folder.entryList(QStringList()<<"cover.png"<<"cover.jpg"<<"cover.gif", QDir::NoFilter, QDir::Name);
        if(covers.count()>0) {
            QPixmap coverImage(QPixmap(QString("%1/%2/%3").arg(libraryPath).arg(folderName).arg(covers[0])));
            comicCover->setPixmap(coverImage.scaled(QSize(256,256), Qt::KeepAspectRatio));
            //comicCover->setPixmap(coverImage);
        } else {
            comicCover->setPixmap(QPixmap(":/Images/magazine_256.png"));
        }

        QStringList folderContents = folder.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files, QDir::Name);
        QRegExp possibleChapterNames("^[^.]*$|.*\\.cbz");
        QStringList chapters = folderContents.filter(possibleChapterNames);

        comicTotalIssues->setText(QString("%1").arg(chapters.count()));

        QChar accessBy = folderName.at(0).toUpper();
        if(accessBy<'A') accessBy='#';
        for(int i=0; i<quickTabs->count(); i++) {
            if(quickTabs->tabText(i) == accessBy) {
                quickTabs->setCurrentIndex(i);
                break;
            }
        }
        comicSource->setEnabled(true);

        QString infoFile = QString("%1/%2/info.ini").arg(libraryPath)
                .arg(libraryData->data(libraryView->currentIndex()).toString());
        QSettings comicInfo(infoFile, QSettings::IniFormat);
        comicSource->setCurrentIndex(
                    comicSource->findText(comicInfo.value(cSettingsKey_source).toString())
                    );
        if(comicSource->currentIndex()<0) {
            comicDisableUpdates->setEnabled(false);
            comicDisableUpdates->setChecked(true);
        } else {
            comicDisableUpdates->setChecked(comicInfo.value(cSettingsKey_updatable, true).toBool());
        }
    } else {
        comicTitle->setText(QString(tr("Local Library")));
        comicCover->setPixmap(QPixmap(":/Images/magazine_256.png"));
        comicsSummary->setText(libraryPath);
        comicSource->setEnabled(false);
        QFileInfo fi(libraryPath);
        comicLastUpdated->setText(fi.lastModified().toLocalTime().toString());
        comicTotalIssues->setText(QString("%1").arg(libraryData->rowCount()));
        comicDisableUpdates->setEnabled(false);
        comicDisableUpdates->setChecked(false);
    }
}

void LocalLibrary::on_libraryView_activated(const QModelIndex &index) {
    QVariant item = libraryData->data( index);
    qDebug() << "read" << item;
}

void LocalLibrary::libraryView_selectionChanged(const QModelIndex &selected, const QModelIndex &deselected) {
    Q_UNUSED(deselected);
    updateComicDescription(selected);
}

void LocalLibrary::on_quickTabs_tabBarClicked(int index) {
    uint row = quickTabs->tabData(index).toUInt();
    libraryView->scrollTo(libraryData->index(row,0));
    libraryView->setCurrentIndex(libraryData->index(row,0));
}

void LocalLibrary::on_comicDisableUpdates_toggled(bool checked) {
    QString infoFile = QString("%1/%2/info.ini").arg(libraryPath)
            .arg(libraryData->data(libraryView->currentIndex()).toString());
    QSettings comicInfo(infoFile, QSettings::IniFormat);
    comicInfo.setValue(cSettingsKey_updatable, checked);
}

void LocalLibrary::on_comicSource_currentTextChanged(const QString &text) {
    QString infoFile = QString("%1/%2/info.ini").arg(libraryPath)
            .arg(libraryData->data(libraryView->currentIndex()).toString());
    QSettings comicInfo(infoFile, QSettings::IniFormat);
    comicInfo.setValue(cSettingsKey_source, text);
}

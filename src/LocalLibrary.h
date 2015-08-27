#ifndef LOCALLIBRARY_H
#define LOCALLIBRARY_H

#include "ui_LocalLibrary.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QFile>

class LocalLibrary : public QWidget, private Ui::LocalLibrary {
    Q_OBJECT

    QString libraryPath;
    QStandardItemModel *libraryData;
    QSortFilterProxyModel *filterLibrary;
    QIcon defaultComicIcon;
    QStandardItemModel *comicsSourceModel; // used to enable/disable sources
    void updateComicDescription(const QModelIndex &index);
    QFile infoFileName(const QString &title);


public:
    explicit LocalLibrary(QWidget *parent = 0);
    bool chooseLibraryRoot();
    bool readLibrary();
    void checkForUpdates(const QString &title = QString());


protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_libraryView_activated(const QModelIndex &index);
    void libraryView_selectionChanged(const QModelIndex &selected, const QModelIndex &deselected);
    void on_iconSizeSelector_valueChanged(int value);
    void on_comicsDisableUpdates_toggled(bool checked);
    void on_comicsSource_currentIndexChanged(const QString &text);
    void on_seriesFilter_textChanged(const QString &arg1);
};

#endif // LOCALLIBRARY_H

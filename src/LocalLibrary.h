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
    QString infoFileName(const QString &title);
    QHash<QString, QVariant> comicsDescription; // description of the current comics


public:
    explicit LocalLibrary(QWidget *parent = 0);
    bool chooseLibraryRoot();
    bool readLibrary();
    void checkAllForUpdates();
    void checkForUpdates(const QString &title);


protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void libraryView_selectionChanged(const QModelIndex &selected, const QModelIndex &deselected);
    void on_iconSizeSelector_valueChanged(int value);
    void on_comicsUpdatable_toggled(bool checked);
    void on_comicsSource_currentIndexChanged(const QString &text);
    void on_seriesFilter_textChanged(const QString &text);
};

#endif // LOCALLIBRARY_H

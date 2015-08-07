#ifndef LOCALLIBRARY_H
#define LOCALLIBRARY_H

#include "ui_LocalLibrary.h"
#include "QStandardItemModel"

class LocalLibrary : public QWidget, private Ui::LocalLibrary {
    Q_OBJECT

    QString libraryPath;
    QStandardItemModel *libraryData;
    QIcon defaultComicIcon;
    void updateComicDescription(const QModelIndex &index);

public:
    explicit LocalLibrary(QWidget *parent = 0);
    bool chooseLibraryRoot();
    bool readLibrary();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_libraryView_activated(const QModelIndex &index);
    void libraryView_selectionChanged(const QModelIndex &selected, const QModelIndex &deselected);
    void on_iconSizeSelector_valueChanged(int value);
    void on_quickTabs_tabBarClicked(int index);
    void on_comicDisableUpdates_toggled(bool checked);
    void on_comicSource_currentTextChanged(const QString &text);
};

#endif // LOCALLIBRARY_H

#define MAIN
#include "main.h"
#include "MainWindow.h"

#include "MangaHere.h"
#include "Batoto.h"
#include "MangaJoy.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                "WOStudio", "ComicsLibrary", &a);

    comicsSources["MangaHere"] = new MangaHere();
    comicsSources["Batoto"] = new Batoto();
    comicsSources["MangaJoy"] = new MangaJoy();

    mainWindow = new MainWindow();
    mainWindow->show();

    progressWindow->setCaption(QObject::tr("Loading cached lists"));
    foreach(QString title, comicsSources.keys()) {
        comicsSources[title]->loadCachedListOfTitles();
    }
    progressWindow->hide();


    if(! localLibraryWindow->readLibrary()) {
        return 1;
    }

    return a.exec();
}

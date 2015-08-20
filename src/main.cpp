#define MAIN
#include "main.h"
#include "MainWindow.h"

#include "MangaHere.h"
#include "Batoto.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                "WOStudio", "ComicsLybrary", &a);
    mainWindow = new MainWindow();
    mainWindow->show();
    if(! localLibraryWindow->readLibrary()) {
        return 1;
    }

    comicsSources["MangaHere"] = new MangaHere();
    comicsSources["Batoto"] = new Batoto();

    return a.exec();
}

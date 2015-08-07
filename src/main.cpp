#define MAIN
#include "main.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                "WOStudio", "ComicsLybrary", &a);
    mainWindow = new MainWindow();
    mainWindow->show();
    if(! localLibraryWindow->readLibrary()) {
        return 1;
    }
    return a.exec();
}

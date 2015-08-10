#ifndef MAIN_H
#define MAIN_H

#ifdef MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

#include <QtCore>
#include <QtGui>
#include <QMdiSubWindow>
#include <QFileDialog>


EXTERN QSettings *settings;
#define settingsKey_iconSize       "iconSize"
#define settingsKey_mainGeometry   "geom"
#define settingsKey_mainState      "state"
#define settingsKey_libraryPath    "library"

#define cSettingsKey_updatable    "update"
#define cSettingsKey_summary      "summary"
#define cSettingsKey_source       "source"
#define cSettingsKey_bookmarks    "bookmarks/%1"

#include "MainWindow.h"
#include "LocalLibrary.h"
#include "ExternalLibrary.h"
EXTERN MainWindow *mainWindow;
EXTERN LocalLibrary *localLibraryWindow;
EXTERN ExternalLibrary *externalLibraryWindow;


#endif // MAIN_H


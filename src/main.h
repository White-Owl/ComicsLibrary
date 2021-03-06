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
#include <QMessageBox>

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
#include "ProgressWindow.h"
#include "NetworkJob.h"

EXTERN MainWindow *mainWindow;
EXTERN LocalLibrary *localLibraryWindow;
EXTERN ExternalLibrary *externalLibraryWindow;
EXTERN ProgressWindow *progressWindow;

EXTERN QHash<QString, ComicsSource*> comicsSources;

/// Utils.cpp
QString decodeHTML(const QString &encoded);
bool readHash(const QString &fileName, QHash<QString, QVariant> &hash);
bool saveHash(const QString &fileName, QHash<QString, QVariant> &hash);

#endif // MAIN_H


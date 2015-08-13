#-------------------------------------------------
#
# Project created by QtCreator 2015-08-04T14:53:34
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComicsLibrary
TEMPLATE = app
QMAKE_CXXFLAGS += -Werror
INCLUDEPATH += ExternalSources

SOURCES += main.cpp\
		MainWindow.cpp \
	LocalLibrary.cpp \
	ExternalLibrary.cpp \
	Utils.cpp \
	ProgressWindow.cpp \
	ExternalSources/Batoto.cpp \
	ExternalSources/MangaHere.cpp

HEADERS  += MainWindow.h \
	main.h \
	LocalLibrary.h \
	ExternalLibrary.h \
	ComicsSource.h \
	ProgressWindow.h \
	ExternalSources/Batoto.h \
	ExternalSources/MangaHere.h

FORMS    += MainWindow.ui \
	LocalLibrary.ui \
	ExternalLibrary.ui \
	ProgressWindow.ui

RESOURCES += \
	resorces.qrc

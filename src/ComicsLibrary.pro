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

SOURCES += main.cpp\
		MainWindow.cpp \
	LocalLibrary.cpp \
	MangaHere.cpp \
    ExternalLibrary.cpp \
    Utils.cpp

HEADERS  += MainWindow.h \
	main.h \
	LocalLibrary.h \
	MangaHere.h \
    ExternalLibrary.h \
    ComicsSource.h

FORMS    += MainWindow.ui \
	LocalLibrary.ui \
    ExternalLibrary.ui

RESOURCES += \
	resorces.qrc


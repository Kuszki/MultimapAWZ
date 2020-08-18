QT			+=	core gui widgets concurrent sql network qml

TARGET		=	AWZ
TEMPLATE		=	app

CONFIG		+=	c++19

SOURCES += \
	awzwidget.cpp \
	connectdialog.cpp \
	importdialog.cpp \
	importworker.cpp \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	awzwidget.hpp \
	commonh.hpp \
	connectdialog.hpp \
	importdialog.hpp \
	importworker.hpp \
	mainwindow.hpp

FORMS += \
	awzwidget.ui \
	connectdialog.ui \
	importdialog.ui \
	mainwindow.ui

TRANSLATIONS += \
	awz_pl.ts

INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include

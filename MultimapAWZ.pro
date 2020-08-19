QT			+=	core gui widgets concurrent sql network qml

TARGET		=	AWZ
TEMPLATE		=	app

CONFIG		+=	c++19

SOURCES += \
	awzwidget.cpp \
	connectdialog.cpp \
	filewidget.cpp \
	importdialog.cpp \
	importworker.cpp \
	main.cpp \
	mainwindow.cpp \
	modeldelegate.cpp \
	modelfilter.cpp \
	ownerwidget.cpp \
	progressdialog.cpp \
	titlewidget.cpp

HEADERS += \
	awzwidget.hpp \
	commonh.hpp \
	connectdialog.hpp \
	filewidget.hpp \
	importdialog.hpp \
	importworker.hpp \
	mainwindow.hpp \
	modeldelegate.hpp \
	modelfilter.hpp \
	ownerwidget.hpp \
	progressdialog.hpp \
	titlewidget.hpp

FORMS += \
	awzwidget.ui \
	connectdialog.ui \
	filewidget.ui \
	importdialog.ui \
	mainwindow.ui \
	ownerwidget.ui \
	progressdialog.ui \
	titlewidget.ui

TRANSLATIONS += \
	awz_pl.ts

INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include

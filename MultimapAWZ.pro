QT			+=	core gui widgets concurrent sql network qml

TARGET		=	AWZ
TEMPLATE		=	app

CONFIG		+=	c++19

SOURCES += \
	awzwidget.cpp \
	connectdialog.cpp \
	editdialog.cpp \
	filewidget.cpp \
	importdialog.cpp \
	importworker.cpp \
	lotwidget.cpp \
	main.cpp \
	mainwindow.cpp \
	modeldelegate.cpp \
	modelfilter.cpp \
	ownerwidget.cpp \
	progressdialog.cpp \
	scanwidget.cpp \
	settingsdialog.cpp \
	titlewidget.cpp

HEADERS += \
	awzwidget.hpp \
	commonh.hpp \
	connectdialog.hpp \
	editdialog.hpp \
	filewidget.hpp \
	importdialog.hpp \
	importworker.hpp \
	lotwidget.hpp \
	mainwindow.hpp \
	modeldelegate.hpp \
	modelfilter.hpp \
	ownerwidget.hpp \
	progressdialog.hpp \
	scanwidget.hpp \
	settingsdialog.hpp \
	titlewidget.hpp

FORMS += \
	awzwidget.ui \
	connectdialog.ui \
	editdialog.ui \
	filewidget.ui \
	importdialog.ui \
	lotwidget.ui \
	mainwindow.ui \
	ownerwidget.ui \
	progressdialog.ui \
	scanwidget.ui \
	settingsdialog.ui \
	titlewidget.ui

TRANSLATIONS += \
	awz_pl.ts

INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include

QT += widgets gui

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    MainWidget.h \
    PathPlanner.h \
    Mapper.h \
    GlobalMapView.h \
    CurrentMapView.h \
    AStar.h

SOURCES += \
    MainWidget.cpp \
    PathPlanner.cpp \
    Mapper.cpp \
    GlobalMapView.cpp \
    CurrentMapView.cpp \
    main.cpp \
    AStar.cpp

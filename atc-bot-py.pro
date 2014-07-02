TEMPLATE = lib
CONFIG += dll
CONFIG += c++11

SOURCES += \
    main.cpp

HEADERS += \
    main.hpp
INCLUDEPATH += ./include/
QMAKE_CXXFLAGS += -O2 -Wall -Wextra

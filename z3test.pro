TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

QMAKE_PRE_LINK  += $$quote(ln -sf /usr/local/lib/libz3.dylib $${OUT_PWD};)
INCLUDEPATH += /usr/local/include
LIBS += -L. -lz3

include($$PWD/../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/vumeter

HEADERS += vumeter.h \
           visualvumeterfactory.h

SOURCES += vumeter.cpp \
           visualvumeterfactory.cpp

RESOURCES += vumeter.qrc

unix{
    target.path = $$PLUGIN_DIR/Visual
    INSTALLS += target
}

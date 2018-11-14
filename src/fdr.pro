QT += core gui

linux {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fdr
TEMPLATE = app

DEFINES += APP_VERSION=\\\"1.0\\\"

INCLUDEPATH += $$PWD \
    $$PWD/helper \
    $$PWD/other \
    $$PWD/threads \
    $$PWD/ui \
    $$PWD/qxtSlider \
    /home/sean/wsp/tensorflow-1.10.0/tensorflow/examples/face

SOURCES += main.cpp \
    helper/MatToQImage.cpp \
    helper/SharedImageBuffer.cpp \
    helper/threadsafequeue.cpp \
    helper/systimer.cpp \
    ui/CameraView.cpp \
    ui/FrameLabel.cpp \
    ui/MainWindow.cpp \
    qxtSlider/qxtglobal.cpp \
    qxtSlider/qxtspanslider.cpp \
    threads/CaptureThread.cpp \
    threads/DetectThread.cpp \
    threads/RecognizeThread.cpp \
    ui/recognitionitem.cpp \
    ui/addpersondialog.cpp


HEADERS += \
    helper/MatToQImage.h \
    helper/SharedImageBuffer.h \
    helper/threadsafequeue.h \
    helper/systimer.h \
    ui/CameraView.h \
    ui/FrameLabel.h \
    ui/MainWindow.h \
    other/Buffer.h \
    other/Config.h \
    other/Structures.h \
    qxtSlider/qxtglobal.h \
    qxtSlider/qxtnamespace.h \
    qxtSlider/qxtspanslider.h \
    qxtSlider/qxtspanslider_p.h \
    threads/RecognizeThread.h \
    threads/DetectThread.h \
    threads/CaptureThread.h \
    ui/recognitionitem.h \
    ui/addpersondialog.h 

FORMS += \
    ui/MainWindow.ui \
    ui/CameraView.ui \
    ui/recognitionitem.ui \
    ui/addpersondialog.ui

QMAKE_CXXFLAGS += -Wall -std=c++11

LIBS += -L/usr/lib -lfacerecog

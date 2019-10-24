#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T19:38:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MechanicalArmPcApplication
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    processing_video_sequences.cpp \
    testapplication_methods.cpp \
    kalma.cpp \
    serialport.cpp

HEADERS  += mainwindow.h \
    camera.h \
    processing_video_sequences.h \
    testapplication_methods.h \
    kalma.h \
    serialport.h

FORMS    += mainwindow.ui


INCLUDEPATH += d:\opencv\build\include\
INCLUDEPATH += d:\opencv\build\include\opencv\
INCLUDEPATH += d:\opencv\build\include\opencv2\


LIBS +=d:\opencv\build\x64\mingw\lib\libopencv_calib3d330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_core_pch_dephelp.a\
       d:\opencv\build\x64\mingw\lib\libopencv_core330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_dnn330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_features2d330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_flann330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_highgui330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_imgcodecs330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_imgproc330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_ml330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_objdetect330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_photo330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_shape330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_stitching330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_superres330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_ts330.a\
       d:\opencv\build\x64\mingw\lib\libopencv_video330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_videoio330.dll.a\
       d:\opencv\build\x64\mingw\lib\libopencv_videostab330.dll.a

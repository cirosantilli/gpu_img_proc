TEMPLATE = app
TARGET = corner_detect_camera
DESTDIR = bin
MOC_DIR = obj
OBJECTS_DIR = obj
LIBS += -L/usr/X11R6/lib/ -lGL -lGLEW -L../../gl_img_helper/lib -lgl_img_helper
QT += opengl
DEPENDPATH += .
INCLUDEPATH += ./include \
			   ../../v4li_camera/include \
			   ../../stages/include \
			   ../../gl_img_helper/include \
               ./obj

HEADERS = ../../v4li_camera/include/v4li_camera.h \
          include/gl_widget.h \
          include/gl_thread.h \

SOURCES = ../../v4li_camera/source/v4li_camera.cpp \
          source/gl_widget.cpp \
          source/gl_thread.cpp \
          source/main.cpp

CONFIG += qt debug

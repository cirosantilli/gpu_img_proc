TEMPLATE = lib
TARGET = gl_img_helper
DESTDIR = lib
MOC_DIR = obj
OBJECTS_DIR = obj
LIBS += -L/usr/X11R6/lib/ -lGL -lGLEW
QT += opengl
DEPENDPATH += .
INCLUDEPATH +=  ./include \
                ../stages/include \
                ../v4li_camera/include \
                include \
                ./obj

HEADERS = ../stages/include/abstract_shader.h \
          ../stages/include/common_shader_code.h \
          ../stages/include/gray_shader.h \
          ../stages/include/rgb_p_shader.h \
          ../stages/include/rgba_gray_shader.h \
          ../stages/include/rgba_shader.h \
          ../stages/include/yuy2_shader.h \
          ../stages/include/gauss_shader.h \
          ../stages/include/edge_shader.h \
          ../stages/include/nmes_shader.h \
          ../stages/include/yuv420_shader.h \
          ../stages/include/yuv420_rgb_p_shader.h \
          ../stages/include/binary_shader.h \
          ../stages/include/matrix_m_shader.h \
          ../stages/include/smooth_m_shader.h \
          ../stages/include/eigen_shader.h \
          ../stages/include/mark_cross_shader.h \
          ../stages/include/nms_threshold_shader.h \
          ../v4li_camera/include/v4li_camera.h \
          include/gl_img_helper.h \
          include/canny.h \
          include/corner.h

SOURCES = ../stages/source/abstract_shader.cpp \
          ../stages/source/common_shader_code.cpp \
          ../stages/source/gray_shader.cpp \
          ../stages/source/rgba_shader.cpp \
          ../stages/source/rgb_p_shader.cpp \
          ../stages/source/rgba_gray_shader.cpp \
          ../stages/source/yuy2_shader.cpp \
          ../stages/source/gauss_shader.cpp \
          ../stages/source/edge_shader.cpp \
          ../stages/source/nmes_shader.cpp \
          ../stages/source/binary_shader.cpp \
          ../stages/source/yuv420_shader.cpp \
          ../stages/source/yuv420_rgb_p_shader.cpp \
          ../stages/source/gray_shader.cpp \
          ../stages/source/matrix_m_shader.cpp \
          ../stages/source/smooth_m_shader.cpp \
          ../stages/source/eigen_shader.cpp \
          ../stages/source/mark_cross_shader.cpp \
          ../stages/source/nms_threshold_shader.cpp \
          ../v4li_camera/source/v4li_camera.cpp \
          source/canny.cpp \
          source/corner.cpp \
          source/gl_img_helper.cpp

CONFIG += qt debug staticlib


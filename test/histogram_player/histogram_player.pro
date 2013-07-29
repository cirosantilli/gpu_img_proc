TEMPLATE = app
TARGET = histogram_player
DESTDIR = bin
MOC_DIR = obj
OBJECTS_DIR = obj
LIBS += -L/usr/X11R6/lib/ -lGL -lGLEW -L../../gl_img_helper/lib -lgl_img_helper
QT += opengl
DEPENDPATH += .
INCLUDEPATH +=  ./include \
                ./obj \
                ../../stages/include \
                ../../gl_img_helper/include

HEADERS = include/gl_widget.h \
          include/gl_thread.h \
          include/offline_widget.h \
          include/histogram.h

SOURCES = source/gl_widget.cpp \
          source/gl_thread.cpp \
          source/offline_widget.cpp \
          source/main.cpp

CONFIG += qt debug

# cuda source
CUDA_SOURCES += source/histogram.cu

# Path to cuda toolkit install
# Location of the CUDA Toolkit binaries and libraries
CUDA_PATH = /usr/local/cuda-5.0
CUDA_INC_PATH = $$CUDA_PATH/include
CUDA_BIN_PATH = $$CUDA_PATH/bin
CUDA_LIB_PATH = $$CUDA_PATH/lib64

# Common binaries
NVCC = $$CUDA_BIN_PATH/nvcc

INCLUDEPATH += $$CUDA_INC_PATH
QMAKE_LIBDIR += $$CUDA_LIB_PATH

# GPU architecture
CUDA_ARCH = sm_20
# NVCC flags
NVCCFLAGS = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v

# Path to libraries
LIBS += -lcudart -lcuda

# join the includes in a line
CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')
cuda.commands = $$NVCC -m64 -O3 -arch=$$CUDA_ARCH -c $$NVCCFLAGS $$CUDA_INC $$LIBS ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
cuda.dependcy_type = TYPE_C
cuda.depend_command = $$CUDA_DIR/bin/nvcc -g -G $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}

cuda.input = CUDA_SOURCES
cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}.o

# Tell Qt that we want add more stuff to the Makefile
QMAKE_EXTRA_COMPILERS += cuda


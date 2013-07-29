#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <GL/glew.h>

#include <QFile>
#include <QTimer>
#include <QGLWidget>

#include <gl_thread.h>

#include <canny.h>
#include <yuy2_shader.h>
#include <gray_shader.h>

class Gl_widget:public QGLWidget
{
    Q_OBJECT
public:
    Gl_widget(int w, int h, const QString& path, QGLFormat& fmt, QWidget *parent = 0);
    ~Gl_widget();

protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);
    void closeEvent(QCloseEvent* event);
    void keyPressEvent(QKeyEvent* event);

    void cleanup();

    void init();
    void init_textures();

protected slots:
    void render_frame(uint8_t* data);

private:
    int v_width;
    int v_height;
    uint8_t* yuv_data;
  
    Canny canny;
    Yuy2_shader ys;
    Gray_shader is;

    GLuint y_tex;
    GLuint uv_tex;
    GLuint edge_tex;

    QMutex mutex;
    Gl_thread thread;
};

#endif


#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <GL/glew.h>

#include <QFile>
#include <QTimer>
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>

#include <gl_thread.h>

#include <corner.h>
#include <yuy2_shader.h>
#include <mark_cross_shader.h>

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
    void init_shaders();
	void init_textures();

	void display_corners();
    
protected slots:
    void render_frame(uint8_t* data);

private:
    GLuint fb_id;

    int v_width;
    int v_height;
    uint8_t* yuv_data;

    GLuint y_tex;
    GLuint uv_tex;
    GLuint corner_tex;

    Yuy2_shader ys;
	Mark_cross_shader mcs;
	
	Corner corner;
	
    QMutex mutex;
    Gl_thread thread;
};

#endif


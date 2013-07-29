#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <GL/glew.h>

#include <QFile>
#include <QTimer>
#include <QGLWidget>

#include <gray_shader.h>
#include <rgba_shader.h>
#include <rgb_p_shader.h>
#include <yuv420_shader.h>
#include <yuv420_rgb_p_shader.h>
#include <offline_widget.h>

class Gl_widget:public QGLWidget
{
    Q_OBJECT
public:
    Gl_widget(int w, int h, const QString& path, QGLFormat& fmt, QWidget *parent = 0);
    ~Gl_widget();

protected:
    void paintGL();
    void initializeGL();
    void closeEvent(QCloseEvent* event);
    void resizeGL(int width, int height);

    void cleanup();

    void init_shaders();
	
    void init_textures();

    void init_hist_buffers();
    void delete_hist_buffers();

    void yuv420_to_rgba();

    void plote_histogram();
   
    void display_rgba_texture();
    
protected slots:
    void time_out();

private:
    GLuint fb_id; 
    GLuint buff_id;
    GLuint prev_id;

    int v_width;
    int v_height;

    GLuint tex_y;
    GLuint tex_u;
    GLuint tex_v;

    GLuint tex_rgba;

    Gray_shader gray;
    Rgba_shader rgba;
    Yuv420_shader yuv420;

    GLuint hist_obj[3];

    QTimer timer;
    Offline_widget offline;
};

#endif


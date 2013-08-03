#include <gl_widget.h>
#include <gl_img_helper.h>

#include <stdint.h>
#include <QKeyEvent>

Gl_widget::Gl_widget(int w, int h, const QString& path, QGLFormat& fmt, QWidget* parent)
    :QGLWidget(fmt, parent)
    , v_width(w)
    , v_height(h)
    , yuv_data(0)
    , canny(w, h, this)
    , ys(this)
    , is(this)
    , y_tex(0)
    , uv_tex(0)
    , edge_tex(0)
    , thread(w, h, path)
{
    setWindowTitle("Camera: "+path);
    setFocusPolicy(Qt::StrongFocus);
}

Gl_widget::~Gl_widget()
{
    cleanup();
}

void Gl_widget::init()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        qDebug() << "Error: glewInit() failed\n";
        exit(0);
    }
    
    bool status = is.init();
    status = status && ys.init();
    status = status && canny.init();
    
    if (false == status)
    {
        exit(0);
    }
    init_textures();
}

void Gl_widget::initializeGL()
{
    init();
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    connect(&thread, SIGNAL(update_widget(uint8_t*)), this, SLOT(render_frame(uint8_t*)));

    thread.start();
}

void Gl_widget::cleanup()
{
    Gl_img_helper::destroy_textures(1, &y_tex);
    Gl_img_helper::destroy_textures(1, &uv_tex);
    Gl_img_helper::destroy_textures(1, &edge_tex);
}

void Gl_widget::init_textures()
{
    Gl_img_helper::create_textures(v_width, v_height, GL_LUMINANCE_ALPHA,
        GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 1, &y_tex);
    Gl_img_helper::create_textures((v_width>>1), v_height, GL_RGBA,
        GL_RGBA, GL_UNSIGNED_BYTE, 1, &uv_tex);
    Gl_img_helper::create_textures(v_width, v_height, GL_R8, GL_RED,
        GL_UNSIGNED_BYTE, 1, &edge_tex);
}

void Gl_widget::paintGL()
{
    if (0 == yuv_data)
    {
        return;
    }

    Gl_img_helper::load_yuy2_texture(yuv_data, v_width, v_height, y_tex, uv_tex);
                        
    canny.edge_approx(y_tex, edge_tex);

    is.display(Gl_img_helper::ST_LT, Gl_img_helper::NO_FLIP, edge_tex);
    ys.display(Gl_img_helper::ST_RT, Gl_img_helper::NO_FLIP, y_tex, uv_tex);
    glFlush();	
}

void Gl_widget::resizeGL(int w, int h)
{
    Gl_img_helper::set_view_port(0, 0, w, h);
}

void Gl_widget::render_frame(uint8_t* data)
{
    mutex.lock();
    yuv_data = data;
    mutex.unlock();
    repaint();
}

void Gl_widget::closeEvent(QCloseEvent* event)
{
    (void)event;
    yuv_data = 0;
    thread.stop();
}

void Gl_widget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Up:
            canny.inc_max_threshold();
            break;
        case Qt::Key_Down:
            canny.dec_max_threshold();
            break;
        case Qt::Key_Left:
            canny.dec_min_threshold();
            break;
        case Qt::Key_Right:
            canny.inc_min_threshold();
            break;
    }
    printf("\nThresholds, min: %d, max: %d", canny.min_threshold(), canny.max_threshold());
    fflush(stdout);
}


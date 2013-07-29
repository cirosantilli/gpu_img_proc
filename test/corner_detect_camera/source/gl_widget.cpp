#include <gl_widget.h>

#include <stdint.h>

#include <QtGui>
#include <QPixmap>

#include <gl_img_helper.h>

Gl_widget::Gl_widget(int w, int h, const QString& path, QGLFormat& fmt, QWidget* parent)
    :QGLWidget(fmt, parent)
    , fb_id(0)
    , v_width(w)
    , v_height(h)
    , yuv_data(0)
    , y_tex(0)
    , uv_tex(0)
    , corner_tex(0)
    , ys(this)
	, mcs(this)
	, corner(w, h, fb_id, this)
    , thread(w, h, path)
{
    setWindowTitle("Corner detection: "+path);
	setFocusPolicy(Qt::StrongFocus);
}

Gl_widget::~Gl_widget()
{
	cleanup();
}

void Gl_widget::cleanup()
{
    Gl_img_helper::destroy_frame_buffer(1, &fb_id);
    Gl_img_helper::destroy_textures(1, &y_tex);
    Gl_img_helper::destroy_textures(1, &uv_tex);
    Gl_img_helper::destroy_textures(1, &corner_tex);
}

void Gl_widget::init_shaders()
{
    bool status = ys.init();
	status = status && mcs.init();
	status = status && corner.init();

    if (false == status)
    {
        exit(0);
    }
}

void Gl_widget::initializeGL()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        qDebug() << "Error: glewInit() failed\n";
        exit(0);
    }

    init_shaders();
    init_textures();
    Gl_img_helper::create_frame_buffer(1, &fb_id);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    connect(&thread, SIGNAL(update_widget(uint8_t*)), this, SLOT(render_frame(uint8_t*)));

    thread.start();
}

void Gl_widget::init_textures()
{
    Gl_img_helper::create_textures(v_width, v_height, GL_LUMINANCE_ALPHA,
        GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 1, &y_tex);
    Gl_img_helper::create_textures((v_width>>1), v_height, GL_RGBA,
        GL_RGBA, GL_UNSIGNED_BYTE, 1, &uv_tex);
    Gl_img_helper::create_textures(v_width, v_height, GL_R8, GL_RED,
        GL_UNSIGNED_BYTE, 1, &corner_tex);
}

void Gl_widget::display_corners()
{
    glEnable(GL_BLEND);	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mcs.display(Gl_img_helper::NORMAL, Gl_img_helper::NO_FLIP, corner_tex);
    glDisable(GL_BLEND);
}

void Gl_widget::paintGL()
{
    if (0 == yuv_data)
    {
        return;
    }

    Gl_img_helper::load_yuy2_texture(yuv_data, v_width, v_height, y_tex, uv_tex);
	corner.detect(y_tex, corner_tex);
	
    Gl_img_helper::set_view_port(0, 0, width(), height());
    
    ys.display(Gl_img_helper::NORMAL, Gl_img_helper::NO_FLIP, y_tex, uv_tex);
    display_corners();
}

void Gl_widget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
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
            corner.inc_threshold();
            break;
        case Qt::Key_Down:
            corner.dec_threshold();
            break;
    }
    printf("\nThreshold: %d (%f)", corner.threshold(), ((float)corner.threshold())/255.0f);
    fflush(stdout);
}

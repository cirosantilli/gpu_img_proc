#include <gl_widget.h>

#include <stdint.h>

#include <QtGui>
#include <QPixmap>

#include <histogram.h>
#include <gl_img_helper.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>

Gl_widget::Gl_widget(int w, int h, const QString& path, QGLFormat& fmt, QWidget* parent)
	:QGLWidget(fmt, parent)
    , fb_id(0)
    , buff_id(0)
    , prev_id(0)
    , v_width(w)
    , v_height(h)
    , tex_y(0)
    , tex_u(0)
    , tex_v(0)
    , tex_rgba(0)
    , gray(this)
    , rgba(this)
    , yuv420(this)
    , timer(this)
    , offline(w, h, path, this, this)
{
    setWindowTitle("Histogram");
	setFocusPolicy(Qt::StrongFocus);
}

Gl_widget::~Gl_widget()
{
	cleanup();
}

void Gl_widget::init_shaders()
{
    bool status = rgba.init();
    status = status && gray.init();
    status = status && yuv420.init();
    if (false == status)
    {
        qDebug() << "init_shaders failed";
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
	
	err = cudaSetDeviceFlags(cudaDeviceBlockingSync);
    if (err != cudaSuccess)
    {
        printf("cudaSetDeviceFlags: %s", cudaGetErrorString(cudaGetLastError()));
        exit(0);
    }
	
    init_shaders();
    init_textures();
    init_hist_buffers();

    Gl_img_helper::create_frame_buffer(1, &fb_id);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    timer.setInterval(42);
    connect(&timer, SIGNAL(timeout()), this, SLOT(time_out()));
    timer.start();
}

void Gl_widget::init_textures()
{
    Gl_img_helper::create_textures(v_width, v_height, GL_LUMINANCE,
        GL_LUMINANCE, GL_UNSIGNED_BYTE, 1, &tex_y);
    Gl_img_helper::create_textures((v_width>>1), (v_height>>1), GL_LUMINANCE,
        GL_LUMINANCE, GL_UNSIGNED_BYTE, 1, &tex_u);
    Gl_img_helper::create_textures((v_width>>1), (v_height>>1), GL_LUMINANCE,
        GL_LUMINANCE, GL_UNSIGNED_BYTE, 1, &tex_v);
    Gl_img_helper::create_textures(v_width, v_height, GL_RGBA, GL_RGBA,
        GL_UNSIGNED_BYTE, 1, &tex_rgba);
}

void Gl_widget::init_hist_buffers()
{
    glGenBuffers(3, hist_obj);    
    for (int i = 0; i < 3; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, hist_obj[i]);
        glBufferData(GL_ARRAY_BUFFER, 256*2*sizeof(GL_FLOAT), 0, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Gl_widget::cleanup()
{
    delete_hist_buffers();
    Gl_img_helper::destroy_textures(1, &tex_y);
    Gl_img_helper::destroy_textures(1, &tex_u);
    Gl_img_helper::destroy_textures(1, &tex_v);
    Gl_img_helper::destroy_textures(1, &tex_rgba);
    Gl_img_helper::destroy_frame_buffer(1, &fb_id);
}

void Gl_widget::delete_hist_buffers()
{
    glDeleteBuffers(3, hist_obj); 
}

void Gl_widget::plote_histogram()
{
    int pos[] = {height()-100, height()-100, height()-100};
    GLfloat color[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    for (int i = 0; i < 3; i++)
    {
        glViewport(width()-250, pos[i], 200, 80);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(+0.0f, +1.0f, 0.0f, +1.0f, +0.0f, +1.0f);
        glMatrixMode(GL_MODELVIEW);

        glBindBuffer(GL_ARRAY_BUFFER, hist_obj[i]);
        glColor3fv(&color[3*i]);
        glVertexPointer(2, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_LINE_STRIP, 0, 256);
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Gl_widget::yuv420_to_rgba()
{
    QVector<Abstract_shader*> pipe;
    pipe.append(&yuv420);
    Gl_img_helper::Tex_info info[] = {{3, {tex_y, tex_u, tex_v}}, {1, {tex_rgba}}};
	Gl_img_helper::set_view_port(0, 0, v_width, v_height);
    Gl_img_helper::process_pipe(fb_id, pipe, info);
}

void Gl_widget::display_rgba_texture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_rgba);
    rgba.render(Gl_img_helper::NORMAL, Gl_img_helper::NO_FLIP);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Gl_widget::paintGL()
{
    if (0 == prev_id || 0 == buff_id)
    {
        return;
    }

    Gl_img_helper::load_yuv420_texture(buff_id, v_width, v_height, tex_y, tex_u, tex_v);
    yuv420_to_rgba();

	Gl_img_helper::set_view_port(0, 0, width(), height());
    display_rgba_texture();
    compute_histogram(tex_rgba, hist_obj, v_width, v_height);

    plote_histogram();

	glFinish();
}

void Gl_widget::resizeGL(int w, int h)
{
    Gl_img_helper::set_view_port(0, 0, w, h);
}

void Gl_widget::time_out()
{
    bool flag = offline.isRenderBufferEmpty();
    buff_id = flag?prev_id:offline.renderBuffer();
    if (0 != prev_id && false == flag)
    {
        offline.addToFreeQueue(prev_id);
    }
    prev_id = buff_id;
    updateGL();
}

void Gl_widget::closeEvent(QCloseEvent* event)
{
    (void)event;
    offline.stop_thread();
}


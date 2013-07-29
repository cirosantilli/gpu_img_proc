#include <offline_widget.h>

#include <gl_thread.h>

Offline_widget::Offline_widget(int width, int height, const QString& path, QGLWidget* shared, QWidget *parent)
    :QGLWidget(parent, shared)
    , flag(true)
    , file_path(path)
    , video_width(width)
    , video_height(height)
    , thread(this)
{
    resize(0, 0);
}

Offline_widget::~Offline_widget()
{
    flag = false;
    glDeleteBuffers(10, buffer);
}

void Offline_widget::paintGL()
{
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f); 
    glVertex2f(1.0f, 1.0f); 
    glVertex2f(-1.0f, 1.0f); 
    glEnd();
}

void Offline_widget::initializeGL()
{
    mutex.lock();
    glGenBuffers(10, buffer);
    for (int i = 0; i < 10; i++)
    {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer[i]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, (video_width*video_height*3)>>1, 0, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        free.enqueue(buffer[i]);
    }
    mutex.unlock();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    thread.start();
}

void Offline_widget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Offline_widget::run()
{
    QFile file(file_path);
    int data_size = (video_width*video_height*3)>>1;
    flag = file.open(QIODevice::ReadOnly);
    if (false == flag)
    {
        qDebug() << "Unable to open file: " << file_path;
    }

    makeCurrent();
    while (flag)
    {
        if (free.isEmpty())
        {
            wc_mutex.lock();
            wc.wait(&wc_mutex);
            wc_mutex.unlock();
        }
        else
        {
            mutex.lock();
            GLuint buff_id = free.dequeue();
            mutex.unlock();
            
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buff_id);
            char* y_data = (char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
            flag = flag && (data_size == file.read(y_data, data_size));
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            
            mutex.lock();
            render.enqueue(buff_id);
            mutex.unlock();
        }
    }
    doneCurrent();

    file.close();

    wc.wakeOne();

    qDebug() << "Exiting thread";
}

GLuint Offline_widget::renderBuffer()
{
    GLuint buff_id;
    mutex.lock();
    buff_id = render.dequeue();
    mutex.unlock();
    return buff_id;
}

bool Offline_widget::isRenderBufferEmpty()
{
    return render.isEmpty();
}

void Offline_widget::addToFreeQueue(GLuint id)
{
    mutex.lock();
    free.enqueue(id);
    mutex.unlock();
    wc.wakeOne();
}

void Offline_widget::stop_thread()
{
    wc.wakeOne();
    flag = false;
    if (thread.isRunning())
    {
        wc_mutex.lock();
        wc.wait(&wc_mutex);
        wc_mutex.unlock();
    }
}


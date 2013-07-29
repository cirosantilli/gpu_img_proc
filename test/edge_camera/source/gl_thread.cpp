#include <GL/glew.h>
#include <gl_thread.h>

#include <QFile>
#include <QtDebug>
#include <QTime>

Gl_thread::Gl_thread(int w, int h, const QString& path)
    :QThread(0)
    , width(w)
    , height(h)
    , exit_flag(false)
    , dev_path(path)
{
}

Gl_thread::~Gl_thread()
{
}

void Gl_thread::run()
{
    uint32_t i = 0;
    uint8_t* data = 0;
    uint32_t fsize = 0;

    bool flag = camera.open(dev_path.toAscii().data());
    flag = flag && camera.set_mode(V4L2_PIX_FMT_YUYV, V4L2_MEMORY_MMAP, width, height);
    flag = flag && camera.set_buffer_count(4);

    for (int i = 0; flag && (i < 4); i++)
    {
        data = camera.get_buffer(i, fsize); 
        flag = camera.enqueue_frame(data, i, fsize);
    }

    flag = flag && camera.start();

    if (false == flag)
    {
        qDebug() << "Unable to use device: " << dev_path;
    }

    while (!exit_flag && flag)
    {
        flag = camera.dqueue_frame(data, i, fsize);
        emit update_widget(data);
        flag = flag && camera.enqueue_frame(data, i, fsize);
    }

    camera.stop();
    camera.close();

    cond.wakeAll();
    qDebug() << "Exiting thread";
    exit_flag = true;
}

void Gl_thread::stop()
{
    if (false == exit_flag)
    {
        exit_flag = true;
        mutex.lock();
        cond.wait(&mutex);
        mutex.unlock();
    }
}


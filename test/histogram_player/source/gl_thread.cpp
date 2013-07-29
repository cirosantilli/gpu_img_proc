#include <GL/glew.h>
#include <offline_widget.h>

#include <QFile>
#include <QtDebug>
#include <QTime>

Gl_thread::Gl_thread(Offline_widget* widget)
    :QThread(0)
    , offline(widget)
{
}

Gl_thread::~Gl_thread()
{
    offline->flag = false;
}

void Gl_thread::run()
{
    sleep(1);
    offline->run();
}

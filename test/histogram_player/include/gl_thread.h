#ifndef _GL_THREAD_H_
#define _GL_THREAD_H_

#include <QThread>

class Offline_widget;

class Gl_thread:public QThread
{
    Q_OBJECT
public:
    Gl_thread(Offline_widget* widget);
    ~Gl_thread();

protected:
    void run();

private:
    Offline_widget* offline;
};


#endif


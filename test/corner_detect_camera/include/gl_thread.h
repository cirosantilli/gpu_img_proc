#ifndef _GL_THREAD_H_
#define _GL_THREAD_H_

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include <v4li_camera.h>

class Gl_thread:public QThread
{
    Q_OBJECT
public:
    Gl_thread(int w, int h, const QString& path);
    ~Gl_thread();

public:
    void stop();
    bool is_running() const { return !exit_flag; };

signals:
    void update_widget(uint8_t* data);

protected:
    void run();

private:
    int width;
    int height;
    bool exit_flag;
    QString dev_path;

    V4li_camera camera;
    
    QMutex mutex;
    QWaitCondition cond;
};


#endif


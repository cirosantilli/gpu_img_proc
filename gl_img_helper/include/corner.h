#ifndef _CORNER_H_
#define _CORNER_H_

#include <QObject>

#include <eigen_shader.h>
#include <matrix_m_shader.h>
#include <smooth_m_shader.h>
#include <nms_threshold_shader.h>

class Corner
{
public:
    Corner(int w, int h, QObject* parent);
    Corner(int w, int h, GLuint& fbid, QObject* parent);
    ~Corner();

public:	
    bool init();
    bool detect(GLuint source, GLuint dest);

    void inc_threshold();
    void dec_threshold();

    int threshold() const;
    void set_threshold(int thresh);
	
protected:
    void create_pipe();
	
protected:
    int width;
    int height;
    bool fb_flag;
    GLuint fbo_id;
    GLuint& fb_id;

    Eigen_shader es;
    Matrix_m_shader mms;
    Smooth_m_shader sms;
    Nms_threshold_shader nts;

    QVector<Abstract_shader*> pipe;
};

#endif

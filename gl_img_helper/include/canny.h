#ifndef _CANNY_H_
#define _CANNY_H_

#include <QObject>

#include <gauss_shader.h>
#include <edge_shader.h>
#include <nmes_shader.h>
#include <binary_shader.h>

class Canny
{
public:
	Canny(int w, int h, QObject* parent);
	Canny(int w, int h, GLuint& fbid, QObject* parent);
	~Canny();

public:	
	bool init();
	bool edge_approx(GLuint source, GLuint dest);
	
	int min_threshold() const;
	int max_threshold() const;
	
    void inc_min_threshold();
    void dec_min_threshold();

    void inc_max_threshold();
    void dec_max_threshold();
	
	void set_min_threshold(int min);
	void set_max_threshold(int max);
	
protected:
	void create_pipe();
	
protected:
	int width;
	int height;
    bool fb_flag;
	GLuint fbo_id;
	GLuint& fb_id;
	
    Edge_shader es;
    Nmes_shader ns;
    Gauss_shader gs;
    Binary_shader bs;

    QVector<Abstract_shader*> pipe;
};

#endif

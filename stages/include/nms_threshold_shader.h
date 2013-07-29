#ifndef _NMS_THRESHOLD_SHADER_H_
#define _NMS_THRESHOLD_SHADER_H_

#include <abstract_shader.h>

class Nms_threshold_shader:public Abstract_shader
{
public:
    Nms_threshold_shader(QObject* parent);
    ~Nms_threshold_shader() {};

public:
    bool init();
    void set_params();
    
    void inc_threshold();
    void dec_threshold();
    
    int get_threshold() const { return threshold; };
    void set_threshold(int thresh) { threshold = thresh; };

protected:
	int threshold;
	
    static const char nms_thresh_fs[];
};

#endif


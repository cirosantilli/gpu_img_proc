#ifndef _YUV420_SHADER_H_
#define _YUV420_SHADER_H_

#include <abstract_shader.h>

class Yuv420_shader:public Abstract_shader
{
public:
    Yuv420_shader(QObject* parent);
    ~Yuv420_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char yuv420_fs[];
};

#endif


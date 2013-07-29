#ifndef _RGBA_GRAY_SHADER_H_
#define _RGBA_GRAY_SHADER_H_

#include <abstract_shader.h>

class Rgba_gray_shader:public Abstract_shader
{
public:
    Rgba_gray_shader(QObject* parent);
    ~Rgba_gray_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char rgba_gray_fs[];
};

#endif


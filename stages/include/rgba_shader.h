#ifndef _RGBA_SHADER_H_
#define _RGBA_SHADER_H_

#include <abstract_shader.h>

class Rgba_shader:public Abstract_shader
{
public:
    Rgba_shader(QObject* parent);
    ~Rgba_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char rgba_fs[];
};

#endif


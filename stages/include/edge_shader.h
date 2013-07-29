#ifndef _EDGE_SHADER_H_
#define _EDGE_SHADER_H_

#include <abstract_shader.h>

class Edge_shader:public Abstract_shader
{
public:
    Edge_shader(QObject* parent);
    ~Edge_shader() {};

public:
    bool init();
    void set_params();

protected:
    static const char edge_fs[];
};

#endif


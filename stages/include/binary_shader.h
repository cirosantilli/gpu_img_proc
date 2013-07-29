#ifndef _BINARY_SHADER_H_
#define _BINARY_SHADER_H_

#include <abstract_shader.h>

class Binary_shader:public Abstract_shader
{
public:
    Binary_shader(QObject* parent);
    ~Binary_shader() {};

public:
    bool init();
    void set_params();

    void inc_min_threshold();
    void dec_min_threshold();

    void inc_max_threshold();
    void dec_max_threshold();

    int min_threshold() const { return min_thr; };
    int max_threshold() const { return max_thr; };

    void set_min_threshold(int thr) { min_thr = thr; };
    void set_max_threshold(int thr) { max_thr = thr; };

private:
    int min_thr;
    int max_thr;

    static const char binary_fs[];
};

#endif

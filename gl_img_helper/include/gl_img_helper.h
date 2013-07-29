#ifndef _GL_IMG_HELPER_H_
#define _GL_IMG_HELPER_H_

#include <GL/glew.h>

#include <QFile>
#include <QTimer>
#include <QGLWidget>

class Abstract_shader;

namespace Gl_img_helper
{
    enum Flip_mode {NO_FLIP, FLIP_VERT, FLIP_HORZ, FLIP_VERT_HORZ};

    enum Disp_mode {ST_LT_TP, ST_RT_TP, ST_LT_BM, ST_RT_BM, NORMAL, ST_LT, ST_RT};

    struct Tex_info
    {
        int count;
        GLuint texture[4];
    };

    void create_frame_buffer(int count, GLuint* fb_id);
    void destroy_frame_buffer(int count, GLuint* fb_id);

    void destroy_textures(int count, GLuint* tex);
    void create_textures(int w, int h, GLint ifmt, GLenum fmt, GLenum type, int count, GLuint* tex);

    void set_view_port(int x, int y, int w, int h);
    
    void render_quad(int loc, Disp_mode dmode, Flip_mode fmode);
    void render_quad(int loc, Disp_mode dmode, Flip_mode fmode, const GLuint* fbo_buffs, int size);
    
    void process_pipe(GLuint fbid, const QVector<Abstract_shader*>& pipe, Tex_info* info);

    void load_yuy2_texture(uint8_t* data, int w, int h, GLuint tex_y, GLuint tex_uv);    
    void load_yuv420_texture(GLuint buff_id, int w, int h, GLuint tex_y, GLuint tex_u, GLuint tex_v);
};

#endif

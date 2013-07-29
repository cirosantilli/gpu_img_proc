#include <gl_img_helper.h>
#include <abstract_shader.h>

static const GLfloat tex_coord[][8] = {
										{0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f}, //Normal
										{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f}, //Vert. flip
										{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f}, //Horz. flip
										{1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f}  //Horz. Vert. Flip
                                      };
                                       
static const GLfloat vertex_coord[][8] = {
											{-1.0f, +1.0f, +0.0f, +1.0f, +0.0f, +0.0f, -1.0f, +0.0f}, //Split Left Top
											{+0.0f, +1.0f, +1.0f, +1.0f, +1.0f, +0.0f, +0.0f, +0.0f}, //Split Right Top
											{-1.0f, +0.0f, +0.0f, +0.0f, +0.0f, -1.0f, -1.0f, -1.0f}, //Split Left Bottom
											{+0.0f, +0.0f, +1.0f, +0.0f, +1.0f, -1.0f, +0.0f, -1.0f}, //Split Right Bottom
											{-1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, -1.0f, -1.0f}, //Full Window
											{-1.0f, +1.0f, +0.0f, +1.0f, +0.0f, -1.0f, -1.0f, -1.0f}, //Split Left
											{+0.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +0.0f, -1.0f}  //Split Right
                                         };

void Gl_img_helper::set_view_port(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f);

    //Initialize modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Gl_img_helper::render_quad(int loc, Disp_mode dmode, Flip_mode fmode)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableVertexAttribArray(loc);
    glVertexPointer(2, GL_FLOAT, 0, vertex_coord[dmode]); 
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coord[fmode]);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, tex_coord[fmode]);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(loc);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Gl_img_helper::render_quad(int loc, Gl_img_helper::Disp_mode dmode, Gl_img_helper::Flip_mode fmode, const GLuint* fbo_buffs, int size)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableVertexAttribArray(loc);
    glVertexPointer(2, GL_FLOAT, 0, vertex_coord[dmode]); 
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coord[fmode]);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, tex_coord[fmode]);    
    if (0 != fbo_buffs)
    {
        glDrawBuffers(size, fbo_buffs); 
    }
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(loc);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Gl_img_helper::process_pipe(GLuint fbid, const QVector<Abstract_shader*>& pipe, Tex_info* info)
{
    GLenum fbo_buffs[] = {
							GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
							GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
                         };

    for (int i = 0; i < pipe.size(); i++)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbid); 
        for (int j = 0; j < info[i+1].count; j++)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+j,
                GL_TEXTURE_2D, info[i+1].texture[j], 0);
        }

        for (int j = 0; j < info[i].count; j++)
        {
            glActiveTexture(GL_TEXTURE0+j);
            glBindTexture(GL_TEXTURE_2D, info[i].texture[j]);
        }

		pipe[i]->render(Gl_img_helper::NORMAL, Gl_img_helper::FLIP_VERT, fbo_buffs, info[i+1].count);
        
        for (int j = 0; j < info[i].count; j++)
        {
            glActiveTexture(GL_TEXTURE0+j);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
}

void Gl_img_helper::create_frame_buffer(int count, GLuint* fb_id)
{
    glGenFramebuffers(count, fb_id);
}

void Gl_img_helper::destroy_frame_buffer(int count, GLuint* fb_id)
{
    glDeleteFramebuffers(count, fb_id);
}

void Gl_img_helper::destroy_textures(int count, GLuint* tex)
{
    glDeleteTextures(count, tex);
}

void Gl_img_helper::create_textures(int w, int h, GLint ifmt,
        GLenum fmt, GLenum type, int count, GLuint* tex)
{   
    glGenTextures(count, tex);
    for (int i = 0; i < count; i++)
    {
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, ifmt, w, h, 0, fmt, type, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Gl_img_helper::load_yuy2_texture(uint8_t* data, int w, int h, GLuint tex_y, GLuint tex_uv)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_y);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_uv);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (w>>1), h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Gl_img_helper::load_yuv420_texture(GLuint buff_id, int w, int h,
                    GLuint tex_y, GLuint tex_u, GLuint tex_v)
{
	int wh = (w >> 1);
	int hh = (h >> 1);
    int u_offset = (w*h);
    int v_offset = u_offset+(u_offset>>2);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buff_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_y);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_u);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, wh, hh, GL_LUMINANCE, GL_UNSIGNED_BYTE, (void*)u_offset);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex_v);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, wh, hh, GL_LUMINANCE, GL_UNSIGNED_BYTE, (void*)v_offset);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

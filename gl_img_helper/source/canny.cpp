#include <canny.h>
#include <gl_img_helper.h>

Canny::Canny(int w, int h, QObject* parent)	
    :width(w)
    , height(h)
    , fb_flag(true)
    , fbo_id(0)
    , fb_id(fbo_id)
    , es(parent)
    , ns(parent)
    , gs(parent)
    , bs(parent)
{
    pipe.clear();
}

Canny::Canny(int w, int h, GLuint& fbid, QObject* parent)
    :width(w)
    , height(h)
    , fb_flag(false)
    , fbo_id(0)
    , fb_id(fbid)
    , es(parent)
    , ns(parent)
    , gs(parent)
    , bs(parent)
{
}

Canny::~Canny()
{
    pipe.clear();
    if (fb_flag)
    {
        Gl_img_helper::destroy_frame_buffer(1, &fb_id);
    }
}

bool Canny::init()
{
    bool status = es.init();
    status = status && ns.init();
    status = status && gs.init();
    status = status && bs.init();
    if (status)
    {
        create_pipe();
        if (fb_flag)
        {
            Gl_img_helper::create_frame_buffer(1, &fb_id);
        }
    }
    return status;
}

bool Canny::edge_approx(GLuint source, GLuint dest)
{
    GLuint tmp[3];

    if (0 == pipe.size())
    {
        return false;
    }

    Gl_img_helper::create_textures(width, height, GL_R16F, GL_RED, GL_FLOAT, 3, tmp);

    Gl_img_helper::Tex_info info[] = {
                                         {1, {source         }},
                                         {1, {tmp[0]         }},
                                         {2, {tmp[1], tmp[2] }},
                                         {1, {tmp[0]         }},
                                         {1, {dest           }}
                                     };

    Gl_img_helper::set_view_port(0, 0, width, height);
    Gl_img_helper::process_pipe(fb_id, pipe, info);
    Gl_img_helper::destroy_textures(3, tmp);

    return true;
}

void Canny::create_pipe()
{
    pipe.append(&gs);
    pipe.append(&es);
    pipe.append(&ns);
    pipe.append(&bs);
}

int Canny::min_threshold() const
{
    return bs.min_threshold();
}

int Canny::max_threshold() const
{
    return bs.max_threshold();
}
	
void Canny::inc_min_threshold()
{
    bs.inc_min_threshold();
}

void Canny::dec_min_threshold()
{
    bs.dec_min_threshold();
}

void Canny::inc_max_threshold()
{
    bs.inc_max_threshold();
}

void Canny::dec_max_threshold()
{
    bs.dec_max_threshold();
}

void Canny::set_min_threshold(int min)
{
    bs.set_min_threshold(min);
}

void Canny::set_max_threshold(int max)
{
    bs.set_max_threshold(max);
}


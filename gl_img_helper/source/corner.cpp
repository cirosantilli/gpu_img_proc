#include <corner.h>
#include <gl_img_helper.h>

Corner::Corner(int w, int h, QObject* parent)	
    :width(w)
    , height(h)
    , fb_flag(true)
    , fbo_id(0)
    , fb_id(fbo_id)
    , es(parent)
    , mms(parent)
    , sms(parent)
    , nts(parent)
{
    pipe.clear();
}

Corner::Corner(int w, int h, GLuint& fbid, QObject* parent)
    :width(w)
    , height(h)
    , fb_flag(false)
    , fbo_id(0)
    , fb_id(fbid)
    , es(parent)
    , mms(parent)
    , sms(parent)
    , nts(parent)
{
}

Corner::~Corner()
{
    pipe.clear();
    if (fb_flag)
    {
        Gl_img_helper::destroy_frame_buffer(1, &fb_id);
    }
}

bool Corner::init()
{
    bool status = es.init();
    status = status && mms.init();
    status = status && sms.init();
    status = status && nts.init();

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

bool Corner::detect(GLuint source, GLuint dest)
{
    GLuint tmp;
    GLuint tmp_rgba[2];

    Gl_img_helper::create_textures(width, height, GL_R16F, GL_RED, GL_FLOAT, 1, &tmp);
    Gl_img_helper::create_textures(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, 2, tmp_rgba);

    Gl_img_helper::Tex_info info[] = {
                                        {1, {source        }},
                                        {1, {tmp_rgba[0]    }},
                                        {1, {tmp_rgba[1]    }},
                                        {1, {tmp            }},
                                        {1, {dest           }}
                                     };

    if (0 == pipe.size())
    {
        create_pipe();
    }

    Gl_img_helper::process_pipe(fb_id, pipe, info);

    Gl_img_helper::destroy_textures(1, &tmp);
    Gl_img_helper::destroy_textures(2, tmp_rgba);

    return true;
}

void Corner::create_pipe()
{
    pipe.append(&mms);
    pipe.append(&sms);
    pipe.append(&es);
    pipe.append(&nts);
}

int Corner::threshold() const
{
    return nts.get_threshold();
}

void Corner::set_threshold(int thresh)
{
    nts.set_threshold(thresh);
}
	
void Corner::inc_threshold()
{
    nts.inc_threshold();
}

void Corner::dec_threshold()
{
    nts.dec_threshold();
}


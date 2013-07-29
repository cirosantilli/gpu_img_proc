#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>

#include <v4li_camera.h>

struct Buffer
{
    uint8_t* start;
    size_t length;
};

V4li_camera::V4li_camera()
    :device(-1)
    , width(0)
    , height(0)
    , memory(0)
    , num_buffers(0)
    , buffer(0)
    , device_name(0)
{
}

V4li_camera::~V4li_camera()
{
    close();
}

bool V4li_camera::open(const char* _name)
{
    close();

    bool flag;
    int length = strlen(_name);
    device_name = new char[length+1];
    strcpy(device_name, _name);

    device = ::open(device_name, O_RDWR|O_NONBLOCK, 0);
    flag = (-1 != device);
    flag = flag && initialize();

    return flag;
}

bool V4li_camera::initialize()
{
    bool flag = (-1 != device);
    if (flag)
    {
        struct v4l2_capability cap;
        struct v4l2_fmtdesc fmt_desc;
        struct v4l2_frmsizeenum frame_sizes;

        flag = ioctl(VIDIOC_QUERYCAP, &cap);
        flag = flag && (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) && (cap.capabilities & V4L2_CAP_STREAMING);

        if (flag)
        {
            bool result = true;
            for (int i = 0; result; i++)
            {
                memset(&fmt_desc, 0, sizeof(fmt_desc));
                fmt_desc.index = i;
                fmt_desc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                result = result && ioctl(VIDIOC_ENUM_FMT, &fmt_desc);
                if (true == result)
                {
                    bool ret = true;
                    for (int j = 0; ret; j++)
                    {
                        memset(&frame_sizes, 0, sizeof(frame_sizes));
                        frame_sizes.index = j;
                        frame_sizes.pixel_format = fmt_desc.pixelformat;
                        ret = ioctl(VIDIOC_ENUM_FRAMESIZES, &frame_sizes);
                        if (true == ret)
                        {
                            frame_params.insert(std::pair<uint32_t, struct v4l2_frmsizeenum>
                                (fmt_desc.pixelformat, frame_sizes));
                        }
                    }
                }
            }
        }
    }
    return flag;
}

void V4li_camera::close()
{
    stop();

    unmap_device();

    if (0 != device_name)
    {
        delete [] device_name;
        device_name = 0;
    }

    if (device >= 0)
    {
        ::close(device);
    }

    device = -1;
}

void V4li_camera::stop()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(VIDIOC_STREAMOFF, &type);
}

bool V4li_camera::start()
{
    bool flag = (-1 != device);
    if (flag)
    {
        enum v4l2_buf_type type;
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        flag = flag && ioctl(VIDIOC_STREAMON, &type);
    }
    return flag;
}

bool V4li_camera::mmap_device()
{
    bool flag = (-1 != device) && (memory == V4L2_MEMORY_MMAP);

    if (flag && num_buffers > 0)
    {
        buffer = new Buffer[num_buffers];
        if (0 != buffer)
        {
            int i;
            struct v4l2_buffer buf;
            for (i = 0; i < (int)num_buffers && flag; ++i)
            {
                memset(&buf, 0, sizeof(buf));
                buf.index = i;
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;

                flag = flag && ioctl(VIDIOC_QUERYBUF, &buf);

                buffer[i].length = buf.length;
                buffer[i].start = (uint8_t *)mmap(0, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, device, buf.m.offset);

                flag = flag && (!(MAP_FAILED == buffer[i].start));
            }

            if (false == flag)
            {
                unmap_device();
            }
        }
    }
    return flag;
}

void V4li_camera::unmap_device()
{
    if (0 != buffer)
    {
        for (int i = 0; i < (int)num_buffers; ++i)
        {
            munmap(buffer[i].start, buffer[i].length);
        }
        num_buffers = 0;

        delete [] buffer; buffer = 0;
    }
}

bool V4li_camera::is_supported(uint32_t f, uint32_t w, uint32_t h)
{
    bool flag = (-1 != device);
    if (flag)
    {
        flag = false;
        std::pair <std::multimap<uint32_t, struct v4l2_frmsizeenum>::iterator, 
            std::multimap<uint32_t, struct v4l2_frmsizeenum>::iterator> ret = frame_params.equal_range(f);
        std::multimap<uint32_t, struct v4l2_frmsizeenum>::iterator itr = ret.first;
        for (; (itr != ret.second) && !flag; ++itr)
        {
            switch (itr->second.type)
            {
                case V4L2_FRMSIZE_TYPE_DISCRETE:
                    flag = (itr->second.discrete.width == w && itr->second.discrete.height == h);
                    break;
                case V4L2_FRMSIZE_TYPE_STEPWISE:
                case V4L2_FRMSIZE_TYPE_CONTINUOUS:
                    flag = (w >= itr->second.stepwise.min_width) && (w <= itr->second.stepwise.max_width) &&
                           (h >= itr->second.stepwise.min_height) && (h <= itr->second.stepwise.max_height);
                    break;
            }
        }
    }
    return flag;
}

bool V4li_camera::set_mode(uint32_t f, uint32_t mem, uint32_t w, uint32_t h)
{
    bool flag = (-1 != device);
    if (flag)
    {
        width = w;
        height = h;
        memory = mem;

        flag = is_supported(f, w, h);

        struct v4l2_format fmt; 
        memset(&fmt, 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = width;
        fmt.fmt.pix.height = height;
        fmt.fmt.pix.pixelformat = f;
        fmt.fmt.pix.field = V4L2_FIELD_NONE;

        flag = flag && ioctl(VIDIOC_S_FMT, &fmt);

        if (false == flag)
        {
            printf("\nInvalid Format or Resolution\nSupported Formats/Resolutions are ...");
            print_formats();
        }
    }
    return flag;
}

bool V4li_camera::set_buffer_count(uint32_t count)
{
    bool flag = (-1 != device);
    if (flag)
    {
        struct v4l2_requestbuffers req;
        memset(&req, 0, sizeof(req));

        req.count = num_buffers = count;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = (v4l2_memory)memory;

        flag = ioctl(VIDIOC_REQBUFS, &req);
        
        if (flag && memory == V4L2_MEMORY_MMAP)
        {
            flag = mmap_device();
        }
    }
    return flag;
}

bool V4li_camera::wait()
{
    fd_set fds;
    int ret = 0;
    struct timeval tv;
    int flag = (-1 != device);

    while (flag)
    {
        FD_ZERO(&fds);
        FD_SET(device, &fds);

        tv.tv_sec = 2;
        tv.tv_usec = 0;

        ret = select(device+1, &fds, 0, 0, &tv);
        if (-1 == ret && EINTR == errno)
        {
            continue;
        }

        if (0 == ret)
        {
            printf("\nTimeout expired");
        }

        flag = 0;
    }

    return (ret > 0);
}

bool V4li_camera::dqueue_frame(uint8_t*& frame, uint32_t& index, uint32_t& size)
{
    bool flag = wait();
    frame = 0;
    if (flag)
    {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.memory = (v4l2_memory)memory;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        flag = ioctl(VIDIOC_DQBUF, &buf);
        index = buf.index;
        size = buf.length;
        if (flag)
        {
            frame = (memory == V4L2_MEMORY_USERPTR)?
                (unsigned char*)buf.m.userptr:buffer[index].start;
        }
    }
    return flag;
}

bool V4li_camera::enqueue_frame(uint8_t* frame, uint32_t index, uint32_t size)
{
    bool flag = (-1 != device);
    if (flag)
    {
        struct v4l2_buffer v4l2_buf;
        memset(&v4l2_buf, 0, sizeof(v4l2_buf));
        v4l2_buf.index = index;
        v4l2_buf.memory = (v4l2_memory)memory;
        v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2_buf.m.userptr = (unsigned long long int)frame;
        v4l2_buf.length = size;
        flag = ioctl(VIDIOC_QBUF, &v4l2_buf);
    }
    return flag;
}

bool V4li_camera::ioctl(int request, void* arg)
{
    return (device == -1)?false:((0 == ::ioctl(device, request, arg))?true:false);
}

uint8_t* V4li_camera::get_buffer(uint32_t i, uint32_t& size)
{
    if (i >= num_buffers || 0 == buffer || memory != V4L2_MEMORY_MMAP)
    {
        size = 0;
        return 0;
    }
    size = buffer[i].length;
    return buffer[i].start;
}

void V4li_camera::print_formats()
{
    std::multimap<uint32_t, struct v4l2_frmsizeenum>::iterator itr = frame_params.begin();
    for (; itr != frame_params.end(); ++itr)
    {
        unsigned char* fourcc = (unsigned char*)&itr->first;
        printf("\nFormat: %c%c%c%c ", fourcc[0], fourcc[1], fourcc[2], fourcc[3]);
        switch (itr->second.type)
        {
            case V4L2_FRMSIZE_TYPE_DISCRETE:
                printf("Discrete (%dx%d)", itr->second.discrete.width, itr->second.discrete.height);
                break;
            case V4L2_FRMSIZE_TYPE_STEPWISE:
                printf("Stepwise Min:(%dx%d) Max:(%dx%d) Step:(%dx%d)", 
                        itr->second.stepwise.min_width, itr->second.stepwise.min_height,
                        itr->second.stepwise.max_width, itr->second.stepwise.max_height,
                        itr->second.stepwise.step_width, itr->second.stepwise.step_height);
                break;
            case V4L2_FRMSIZE_TYPE_CONTINUOUS:
                printf("Continous Min:(%dx%d) Max:(%dx%d)", 
                        itr->second.stepwise.min_width, itr->second.stepwise.min_height,
                        itr->second.stepwise.max_width, itr->second.stepwise.max_height);
                break;
        }
    }
    printf("\n");
}


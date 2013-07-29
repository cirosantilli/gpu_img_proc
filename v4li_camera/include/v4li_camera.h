#ifndef _V4LI_CAMERA_H_
#define _V4LI_CAMERA_H_

#include <stdint.h>
#include <linux/videodev2.h>

#include <map>

struct Buffer;

class V4li_camera
{
public:
    V4li_camera();
    ~V4li_camera();

public:
    bool open(const char* name);
    void close();

    bool is_supported(uint32_t f, uint32_t w, uint32_t h);
    bool set_mode(uint32_t f, uint32_t mem, uint32_t w, uint32_t h);

    bool set_buffer_count(uint32_t count);
    uint32_t get_buffer_count() const { return num_buffers; };

    //Use this method only for V4L2_MEMORY_MMAP memory
    uint8_t* get_buffer(uint32_t i, uint32_t& size);

    void stop();
    bool start();

    //Don't pass user allocated pointer for V4L2_MEMORY_MMAP mode (pass only the mapped memory)
    bool enqueue_frame(uint8_t* frame, uint32_t index, uint32_t size);
    bool dqueue_frame(uint8_t*& frame, uint32_t& index, uint32_t& size);

    void print_formats();

    const std::multimap<uint32_t, struct v4l2_frmsizeenum>& frame_parameters() { return frame_params; };

protected:
    bool wait();
    bool initialize();
    bool mmap_device();
    void unmap_device();
    bool ioctl(int request, void* arg);

protected:
    int device;
    uint32_t width;
    uint32_t height;
    uint32_t memory;
    uint32_t num_buffers;

    Buffer* buffer;
    char* device_name;

    std::multimap<uint32_t, struct v4l2_frmsizeenum> frame_params;
};

#endif


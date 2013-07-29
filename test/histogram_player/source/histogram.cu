#include <stdio.h>
#include <stdint.h>

#include <cuda.h>
#include <cuda_runtime_api.h>

#include <histogram.h>
#include <cuda_gl_interop.h>

texture<uint8_t, 2, cudaReadModeElementType> tex_r;
texture<uchar4, 2, cudaReadModeElementType> tex_rgba;

__global__ void comp_histogram(GLint* hist)
{
    uint8_t red;
    int i, x, y;
    __shared__ int temp_hist[256];

    i = (blockDim.x*threadIdx.y)+threadIdx.x;

    temp_hist[i] = 0;

    __syncthreads();
  
    x = (blockIdx.x*blockDim.x+threadIdx.x);
    y = (blockIdx.y*blockDim.y+threadIdx.y);

    red = tex2D(tex_r, x, y);

    atomicAdd(&temp_hist[red], 1);

    __syncthreads();

    atomicAdd(&hist[i], temp_hist[i]);
}

__global__ void comp_histogram(GLint* hist_r, GLint* hist_g, GLint* hist_b)
{
    int i, x, y;
    uint8_t red, green, blue;

    __shared__ int temp_hist_r[256];
    __shared__ int temp_hist_g[256];
    __shared__ int temp_hist_b[256];

    i = (blockDim.x*threadIdx.y)+threadIdx.x;

    temp_hist_r[i] = 0;
    temp_hist_g[i] = 0;
    temp_hist_b[i] = 0;

    __syncthreads();
  
    x = (blockIdx.x*blockDim.x+threadIdx.x);
    y = (blockIdx.y*blockDim.y+threadIdx.y);

    red = tex2D(tex_rgba, x, y).x;
    green = tex2D(tex_rgba, x, y).y;
    blue = tex2D(tex_rgba, x, y).z;

    atomicAdd(&temp_hist_r[red], 1);
    atomicAdd(&temp_hist_g[green], 1);
    atomicAdd(&temp_hist_b[blue], 1);

    __syncthreads();

    atomicAdd(&hist_r[i], temp_hist_r[i]);
    atomicAdd(&hist_g[i], temp_hist_g[i]);
    atomicAdd(&hist_b[i], temp_hist_b[i]);
}

__global__ void max_value(GLint* hist, GLint* max, GLint count)
{
    int p, q;
    int i, j, k, l;
    __shared__ int temp[2][256];
    
    temp[0][threadIdx.x] = hist[threadIdx.x];

    __syncthreads();

    i = 0;
    j = 1;
    l = count/2;
    while (l > 0)
    {
        if (threadIdx.x < l)
        {
            p = (2*threadIdx.x); q = p+1;
            temp[j][threadIdx.x] = (temp[i][p] < temp[i][q])?temp[i][q]:temp[i][p];
        }
        l /= 2;
        k = i;
        i = j;
        j = k;
        __syncthreads();
    }
    *max = temp[i][0];
}

__global__ void convert_to_coords(GLint* hist, GLfloat* coord, GLint* max)
{
    int p, q;
    p = (2*threadIdx.x); q = p+1;
    coord[p] = ((GLfloat)threadIdx.x)/256.0f;
    coord[q] = ((GLfloat)hist[threadIdx.x])/((GLfloat)*max);
}


void compute_histogram(unsigned int texture, unsigned int hist_obj, int width, int height)
{
    cudaError_t err;
    GLfloat* dev_hist = 0;

    cudaArray* array;
    cudaGraphicsResource* res;

    cudaGLRegisterBufferObject(hist_obj);
    cudaGLMapBufferObject((void **)&dev_hist, hist_obj);

    err = cudaGraphicsGLRegisterImage(&res, texture, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsReadOnly);
    if (err != cudaSuccess)
    {
        printf("cudaGraphicsGLRegisterImage Failed: %s", cudaGetErrorString(cudaGetLastError()));
        exit(0);
    }

    cudaGraphicsMapResources(1, &res);

    cudaChannelFormatDesc chan_desc = cudaCreateChannelDesc<uint8_t>();
    err = cudaGraphicsSubResourceGetMappedArray(&array, res, 0, 0);
    if (err != cudaSuccess)
    {
        printf("cudaGraphicsSubResourceGetMappedArray Failed: %s", cudaGetErrorString(cudaGetLastError()));
        exit(0);
    }

    if (cudaBindTextureToArray(&tex_r, array, &chan_desc) != cudaSuccess) {
	    printf("Failed to bind texture - %s\n", cudaGetErrorString(cudaGetLastError()));
	    exit(0);
    }

    GLint* dev_hist_mem = 0;
    cudaMalloc(&dev_hist_mem, (1+256)*sizeof(GLint));
    cudaMemset(dev_hist_mem, 0, (1+256)*sizeof(GLint)); 

    dim3 bsize(16, 16);
    dim3 gsize(width/bsize.x, height/bsize.y);
    comp_histogram<<<gsize, bsize>>>(dev_hist_mem);
    max_value<<<1, 256>>>(dev_hist_mem, &dev_hist_mem[256], 256);
    convert_to_coords<<<1, 256>>>(dev_hist_mem, dev_hist, &dev_hist_mem[256]);
    cudaThreadSynchronize();

    cudaFree(dev_hist_mem);

    cudaUnbindTexture(tex_r);
    cudaGLUnmapBufferObject(hist_obj);
    cudaGLUnregisterBufferObject(hist_obj);
    
    cudaGraphicsUnmapResources(1, &res);
    cudaGraphicsUnregisterResource(res);
}

void compute_histogram(unsigned int texture, unsigned int* hist_obj, int width, int height)
{
    cudaError_t err;
    GLfloat* dev_hist[3] = {0, 0, 0};

    cudaArray* array;
    cudaGraphicsResource* res;

    for (int i = 0; i < 3; i++)
    {
        cudaGLRegisterBufferObject(hist_obj[i]);
        cudaGLMapBufferObject((void **)&dev_hist[i], hist_obj[i]);
    }

    err = cudaGraphicsGLRegisterImage(&res, texture, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsReadOnly);
    if (err != cudaSuccess)
    {
        printf("cudaGraphicsGLRegisterImage Failed: %s", cudaGetErrorString(cudaGetLastError()));
        exit(0);
    }

    cudaGraphicsMapResources(1, &res);

    cudaChannelFormatDesc chan_desc = cudaCreateChannelDesc<uchar4>();
    err = cudaGraphicsSubResourceGetMappedArray(&array, res, 0, 0);
    if (err != cudaSuccess)
    {
        printf("cudaGraphicsSubResourceGetMappedArray Failed: %s", cudaGetErrorString(cudaGetLastError()));
        exit(0);
    }

    if (cudaBindTextureToArray(&tex_rgba, array, &chan_desc) != cudaSuccess) {
	    printf("Failed to bind texture - %s\n", cudaGetErrorString(cudaGetLastError()));
	    exit(0);
    }

    GLint* dev_hist_mem = 0;
    cudaMalloc(&dev_hist_mem, (3+(256*3))*sizeof(GLint));
    cudaMemset(dev_hist_mem, 0, (3+(256*3))*sizeof(GLint));

    cudaStream_t stream1, stream2, stream3;
    cudaStreamCreate(&stream1); 
    cudaStreamCreate(&stream2); 
    cudaStreamCreate(&stream3); 

    dim3 bsize(16, 16);
    dim3 gsize(width/bsize.x, height/bsize.y);
    comp_histogram<<<gsize, bsize>>>(dev_hist_mem, &dev_hist_mem[256], &dev_hist_mem[512]);
    max_value<<<1, 256, 0, stream1>>>(dev_hist_mem, &dev_hist_mem[768], 256);
    max_value<<<1, 256, 0, stream2>>>(&dev_hist_mem[256], &dev_hist_mem[769], 256);
    max_value<<<1, 256, 0, stream3>>>(&dev_hist_mem[512], &dev_hist_mem[770], 256);
    convert_to_coords<<<1, 256, 0, stream1>>>(dev_hist_mem, dev_hist[0], &dev_hist_mem[768]);
    convert_to_coords<<<1, 256, 0, stream2>>>(&dev_hist_mem[256], dev_hist[1], &dev_hist_mem[769]);
    convert_to_coords<<<1, 256, 0, stream3>>>(&dev_hist_mem[512], dev_hist[2], &dev_hist_mem[770]);
    //cudaThreadSynchronize();

    cudaFree(dev_hist_mem);
    cudaStreamDestroy(stream1); 
    cudaStreamDestroy(stream2);
    cudaStreamDestroy(stream3);

    cudaUnbindTexture(tex_rgba);
    for (int i = 0; i < 3; i++)
    {
        cudaGLUnmapBufferObject(hist_obj[i]);
        cudaGLUnregisterBufferObject(hist_obj[i]);
    }

    cudaGraphicsUnmapResources(1, &res);
    cudaGraphicsUnregisterResource(res);
}

void print_cuda_device_info()
{
    int count = 0;
    cudaDeviceProp prop;

    cudaGetDeviceCount(&count);

    printf("\nCUDA Device Count: %d", count); 
    for (int i = 0; i < count; i++)
    {
        cudaGetDeviceProperties(&prop, i);
        printf("\nDevice: %d", i);
        printf("\nName: %s", prop.name);
        printf("\nRevision: Major: %d, Minor: %d", prop.major, prop.minor);
        printf("\nWarp Size: %d", prop.warpSize);
        printf("\nMemory Bus width: %d", prop.memoryBusWidth);
        printf("\nMemory Clock Rate: %d", prop.memoryClockRate);
        printf("\nConcurrent Kernels: %d", prop.concurrentKernels);
        printf("\nMultiprocessor count: %d", prop.multiProcessorCount);
        printf("\nTotal Global Memory: %d", (int)prop.totalGlobalMem);
        printf("\nTotal Constant Memory: %d", (int)prop.totalConstMem);
        printf("\nShared Memory per Block: %d", (int)prop.sharedMemPerBlock);
        printf("\nMax grid dimensions: (%d, %d, %d)", prop.maxGridSize[0], 
                                                       prop.maxGridSize[1], 
                                                       prop.maxGridSize[2]);
        printf("\nMax threads per block: %d", prop.maxThreadsPerBlock);
        printf("\nMax threads dimensions: (%d, %d, %d)\n", prop.maxThreadsDim[0], 
                                                       prop.maxThreadsDim[1], 
                                                       prop.maxThreadsDim[2]);
    }
}


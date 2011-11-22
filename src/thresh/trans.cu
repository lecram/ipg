#include "../ipg.h"
#include "trans.h"

__global__ void
thresh_pixel(unsigned char *img, int width, int height,
             unsigned char threshold,
             unsigned char below, unsigned char above,
             int keep)
{
    int x, y;
    int offset;

    x = blockIdx.x * blockDim.x + threadIdx.x;
    y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height)
        return;

    offset = y * width + x;

    if (img[offset] <= threshold) {
        if (!(keep & KEEP_BELOW))
            img[offset] = below;
    }
    else {
        if (!(keep & KEEP_ABOVE))
            img[offset] = above;
    }
}

extern "C"
void
thresh(Matrix *matrix, unsigned char threshold,
       unsigned char below, unsigned char above,
       int keep)
{
    unsigned char *d_img;
    size_t size;
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((matrix->width + threadsPerBlock.x - 1) / threadsPerBlock.x,
                   (matrix->height + threadsPerBlock.y - 1) / threadsPerBlock.y);

    size = matrix->width * matrix->height;
    cudaMalloc((void **) &d_img, size);
    cudaMemcpy(d_img, matrix->data, size, cudaMemcpyHostToDevice);
    thresh_pixel<<<numBlocks, threadsPerBlock>>>(
        d_img, matrix->width, matrix->height, threshold, below, above, keep
    );
    cudaMemcpy(matrix->data, d_img, size, cudaMemcpyDeviceToHost);
    cudaFree(d_img);
}

__global__ void
thresh_pixelN(unsigned char *img, int width, int height,
              unsigned char threshold,
              unsigned char below, unsigned char above,
              int keep)
{
    int x, y, z;
    int offset;
    int size;

    size = width * height;
    x = blockIdx.x * blockDim.x + threadIdx.x;
    y = blockIdx.y * blockDim.y + threadIdx.y;
    offset = y * width + x;
    z = offset / size;
    x %= width;
    y %= height;
    if (z >= N)
        return;


    if (img[offset] <= threshold) {
        if (!(keep & KEEP_BELOW))
            img[offset] = below;
    }
    else {
        if (!(keep & KEEP_ABOVE))
            img[offset] = above;
    }
}

extern "C"
void
threshN(Matrix *matrix, unsigned char threshold,
        unsigned char below, unsigned char above,
        int keep)
{
    unsigned char *d_img;
    size_t size;
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((matrix->width * N + threadsPerBlock.x - 1) / threadsPerBlock.x,
                   (matrix->height * N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    size = matrix->width * matrix->height * N;
    cudaMalloc((void **) &d_img, size);
    cudaMemcpy(d_img, matrix->data, size, cudaMemcpyHostToDevice);
    thresh_pixelN<<<numBlocks, threadsPerBlock>>>(
        d_img, matrix->width, matrix->height, threshold, below, above, keep
    );
    cudaMemcpy(matrix->data, d_img, size, cudaMemcpyDeviceToHost);
    cudaFree(d_img);
}

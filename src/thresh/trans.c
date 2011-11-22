#include "../ipg.h"
#include "trans.h"

void
thresh(Matrix *matrix, unsigned char threshold,
       unsigned char below, unsigned char above,
       int keep)
{
    int x, y;
    int offset;

    for (y = 0; y < matrix->height; y++) {
        for (x = 0; x < matrix->width; x++) {
            offset = y * matrix->width + x;

            if (matrix->data[offset] <= threshold) {
                if (!(keep & KEEP_BELOW))
                    matrix->data[offset] = below;
            }
            else {
                if (!(keep & KEEP_ABOVE))
                    matrix->data[offset] = above;
            }
        }
    }
}

void
threshN(Matrix *matrix[], unsigned char threshold,
        unsigned char below, unsigned char above,
        int keep)
{
    int x, y;
    int offset;
    int i;

    for (i = 0; i < N; i++) {
        for (y = 0; y < matrix[i]->height; y++) {
            for (x = 0; x < matrix[i]->width; x++) {
                offset = y * matrix[i]->width + x;

                if (matrix[i]->data[offset] <= threshold) {
                    if (!(keep & KEEP_BELOW))
                        matrix[i]->data[offset] = below;
                }
                else {
                    if (!(keep & KEEP_ABOVE))
                        matrix[i]->data[offset] = above;
                }
            }
        }
    }
}

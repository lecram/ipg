#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ipg.h"

int
main(int argc, char *argv[])
{
    IplImage *image = NULL;
    char *infile, *outfile;
    Matrix matrix;
    Args args;
    int64 t0, t1;
    double tps, deltatime;
    int i;

    infile = argv[1];
    outfile = argv[2];
    image = cvLoadImage(infile, CV_LOAD_IMAGE_GRAYSCALE);
    if (image == NULL) {
        printf("Could not load image \"%s\".\n", infile);
        return EXIT_FAILURE;
    }
    matrix.data = (unsigned char *) image->imageData;
    matrix.width = image->width;
    matrix.height = image->height;
    args.c = argc - 3;
    for (i = 0; i < 3; i++)
        args.v[i] = argv[i + 3];
    t0 = cvGetTickCount();
    proc(&matrix, &args);
    t1 = cvGetTickCount();
    tps = cvGetTickFrequency() * 1.0e6;
    deltatime = (double) (t1 - t0) / tps;

    cvSaveImage(outfile, image, 0);
    printf("%dx%d image processed in %.3f seconds.\n",
           image->width, image->height, deltatime);
    cvReleaseImage(&image);
    return EXIT_SUCCESS;
}

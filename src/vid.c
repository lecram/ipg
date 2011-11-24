#include <stdlib.h>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ipg.h"

int
main(int argc, char *argv[])
{
    CvCapture *capture = NULL;
    IplImage *src_frame, *image, *dst_frame;
    char *infile, *outfile;
    Matrix matrix;
    Args args;
    int64 t0, t1;
    double tps, deltatime;
    CvVideoWriter *writer;
    CvSize size;
    double fps;
    int frame_count;
    int i;

    infile = argv[1];
    outfile = argv[2];
    args.c = argc - 3;
    for (i = 0; i < 3; i++)
        args.v[i] = argv[i + 3];
    capture = cvCaptureFromFile(infile);
    if (capture == NULL) {
        printf("Could not load video \"%s\".\n", infile);
        return EXIT_FAILURE;
    }
    src_frame = cvQueryFrame(capture);
    fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    size = cvSize(
        (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
        (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
    );
    writer = cvCreateVideoWriter(outfile, CV_FOURCC('I', '4', '2', '0'), fps, size, 1);
    printf("Saving to \"%s\"...\n", outfile);
    image = cvCreateImage(size, IPL_DEPTH_8U, 1);
    dst_frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
    matrix.width = dst_frame->width;
    matrix.height = dst_frame->height;
    frame_count = 0;
    t0 = cvGetTickCount();
    while ((src_frame = cvQueryFrame(capture)) != NULL) {
        cvCvtColor(src_frame, image, CV_BGR2GRAY);
        matrix.data = (unsigned char *) image->imageData;
        proc(&matrix, &args);
        cvCvtColor(image, dst_frame, CV_GRAY2BGR);
        cvWriteFrame(writer, dst_frame);
        frame_count++;
    }
    t1 = cvGetTickCount();
    tps = cvGetTickFrequency() * 1.0e6;
    deltatime = (double) (t1 - t0) / tps;
    printf("%d frames of %dx%d processed in %.3f seconds.\n",
           frame_count, dst_frame->width, dst_frame->height,
           deltatime);
    cvReleaseVideoWriter(&writer);
    cvReleaseImage(&dst_frame);
    cvReleaseCapture(&capture);
    return EXIT_SUCCESS;
}

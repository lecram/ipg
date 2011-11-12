#include <stdlib.h>
#include <stdio.h>

#include <opencv/cv.>
#include <opencv/highgui.h>

#include "ipg.h"

int
main(int argc, char *argv[])
{
    CvCapture *capture = NULL;
    IplImage *src_frame, *dst_frame;
    char *infile, *outfile;
    Matrix matrix;
    Args args;
    int64 t0, t1;
    double tps, deltatime;
    CvVideoWriter *writer;
    CvSize size;
    double fps;

    infile = argv[1];
    outfile = argv[2];
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
    writer = cvCreateVideoWriter(outfile, CV_FOURCC('M', 'J', 'P', 'G'), fps, size, 1);
    printf("Saving to \"%s\".", outfile);
    dst_frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
    while ((src_frame = cvQueryFrame(capture)) != NULL) {
        printf(".");
        /* ToDo: copy src_frame to dst_frame and call transformation. */
        cvWriteFrame(writer, dst_frame);
    }
    puts("");
    cvReleaseVideoWriter(&writer);
    cvReleaseImage(&dst_frame);
    cvReleaseCapture(&capture);
    return EXIT_SUCCESS;
}

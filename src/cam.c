#include <stdlib.h>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ipg.h"

#define FPS_RES 32

int
main(int argc, char *argv[])
{
    CvCapture *capture = NULL;
    IplImage *frame, *gsframe;
    Matrix matrix;
    Args args;
    CvFont fouter, finner;
    CvSize textsize;
    int baseline, border;
    CvPoint torg;
    CvScalar ocolor, icolor;
    char text[] = "    fps";
    int fcount;
    long int fps;
    int64 t0, t1;

    args.c = argc - 1;
    args.v = argv + 1;
    capture = cvCaptureFromCAM(CV_CAP_ANY);
    if (capture == NULL) {
        puts("Could not capture from webcam.");
        return EXIT_FAILURE;
    }
    cvInitFont(&fouter, CV_FONT_HERSHEY_SIMPLEX, 0.7, 0.7, 0, 3, 8);
    cvInitFont(&finner, CV_FONT_HERSHEY_SIMPLEX, 0.7, 0.7, 0, 1, 8);
    border = 6;
    ocolor = cvScalar(  0,   0,   0, 255);
    icolor = cvScalar(255, 255, 255, 255);
    fcount = 0;
    fps = 0;
    frame = cvQueryFrame(capture);
    matrix.width = frame->width;
    matrix.height = frame->height;
    gsframe = cvCreateImage(cvSize(frame->width, frame->height),
                            IPL_DEPTH_8U, 1);
    cvNamedWindow("Cam", CV_WINDOW_AUTOSIZE);
    t0 = cvGetTickCount();
    while (1) {
        frame = cvQueryFrame(capture);
        cvCvtColor(frame, gsframe, CV_RGB2GRAY);
        matrix.data = gsframe->imageData;
        proc(&matrix, &args);
        cvGetTextSize(text, &fouter, &textsize, &baseline);
        torg.x = frame->width - textsize.width - border;
        torg.y = textsize.height + border;
        cvPutText(gsframe, text, torg, &fouter, ocolor);
        cvPutText(gsframe, text, torg, &finner, icolor);
        cvShowImage("Cam", gsframe);
        if ((cvWaitKey(10) & 255) == 27) break;
        fcount++;
        if (fcount == FPS_RES) {
            t1 = cvGetTickCount();
            fps = FPS_RES / ((t1 - t0) * 1e-6 / cvGetTickFrequency());
            text[0] = fps >= 1000 ? fps % 10000 / 1000 + '0' : ' ';
            text[1] = fps >=  100 ? fps %  1000 /  100 + '0' : ' ';
            text[2] = fps >=   10 ? fps %   100 /   10 + '0' : ' ';
            text[3] = fps >=    1 ? fps %    10 /    1 + '0' : ' ';
            fcount = 0;
            t0 = t1;
        }
    }
    cvReleaseImage(&gsframe);
    cvReleaseCapture(&capture);
    cvDestroyWindow("Cam" );
    return 0;
}

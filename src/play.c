#include <stdlib.h>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>


int
main(int argc, char *argv[])
{
    CvCapture *capture = NULL;
    IplImage *frame;
    char *infile;
    double fps;
    double wait;

    infile = argv[1];
    capture = cvCaptureFromFile(infile);
    if (capture == NULL) {
        printf("Could not load video \"%s\".\n", infile);
        return EXIT_FAILURE;
    }
    fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    wait = 1000 / fps;
    cvNamedWindow(infile, 1);
    while ((frame = cvQueryFrame(capture)) != NULL) {
        cvShowImage(infile, frame);
        if(cvWaitKey(wait) != -1)
            break;
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow(infile);
    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int
imgcmp(IplImage *img_a, IplImage *img_b, IplImage *img_diff)
{
    int width, height;
    int x, y;
    int offset;
    char *a, *b, *diff;
    int count;

    width = img_a->width;
    height = img_a->height;
    a = img_a->imageData;
    b = img_b->imageData;
    diff = img_diff->imageData;
    count = 0;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            offset = y * width + x;
            if (a[offset] != b[offset]) {
                diff[offset] = 0;
                count++;
            }
            else
                diff[offset] = 255;
        }
    }
    return count;
}

int
main(int argc, char *argv[])
{
    IplImage *img_a, *img_b, *img_diff;
    int count;

    if (argc != 4) {
        puts("Usage: imgcmp img_a img_b img_diff");
        return EXIT_FAILURE;
    }
    img_a = img_b = NULL;
    img_a = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if (img_a == NULL) {
        printf("Could not load image \"%s\".", argv[1]);
        return EXIT_FAILURE;
    }
    img_b = cvLoadImage(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
    if (img_b == NULL) {
        printf("Could not load image \"%s\".", argv[2]);
        return EXIT_FAILURE;
    }

    if (img_a->width != img_b->width  ||  img_a->height != img_b->height) {
        printf("Cannot compare images of different sizes (%dx%d and %dx%d).\n",
               img_a->width, img_a->height, img_b->width, img_b->height);
        return EXIT_FAILURE;
    }
    img_diff = cvCreateImage(cvSize(img_a->width, img_a->height), IPL_DEPTH_8U, 1);
    count = imgcmp(img_a, img_b, img_diff);
    if (count)
        printf("%s != %s\n", argv[1], argv[2]);
    else
        printf("%s == %s\n", argv[1], argv[2]);
    printf("diff count: %d / (%dx%d)\n", count, img_a->width, img_a->height);
    cvSaveImage(argv[3], img_diff, 0);
    cvReleaseImage(&img_a);
    cvReleaseImage(&img_b);
    cvReleaseImage(&img_diff);
    return EXIT_SUCCESS;
}

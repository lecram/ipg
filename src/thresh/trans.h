#define KEEP_NONE  0
#define KEEP_BELOW 1
#define KEEP_ABOVE 2
#define KEEP_BOTH  3

void thresh(Matrix *matrix, unsigned char threshold,
            unsigned char below, unsigned char above,
            int keep);

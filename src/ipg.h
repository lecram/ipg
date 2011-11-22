typedef struct {
    unsigned char *data;
    int width;
    int height;
} Matrix;

typedef struct {
    int c;
    char *v[32];
} Args;

void proc(Matrix *matrix, Args *args);

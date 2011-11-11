typedef struct {
    unsigned char *data;
    int width;
    int height;
} Matrix;

typedef struct {
    int c;
    char **v;
} Args;

void proc(Matrix *matrix, Args *args);

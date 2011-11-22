#include <stdlib.h>
#include <string.h>

#include "../ipg.h"
#include "trans.h"

void
proc(Matrix *matrix, Args *args)
{
    double threshold, below, above;
    unsigned char uc_threshold, uc_below, uc_above;
    int keep;

    below = above = 0.0;
    keep = KEEP_NONE;
    threshold = strtod(args->v[0], NULL);
    if (!strcmp(args->v[1], "="))
        keep |= KEEP_BELOW;
    else
        below = strtod(args->v[1], NULL);
    if (!strcmp(args->v[2], "="))
        keep |= KEEP_ABOVE;
    else
        above = strtod(args->v[2], NULL);
    uc_threshold = (unsigned char) (threshold * 255.0);
    uc_below = (unsigned char) (below * 255.0);
    uc_above = (unsigned char) (above * 255.0);
    thresh(matrix, uc_threshold, uc_below, uc_above, keep);
}

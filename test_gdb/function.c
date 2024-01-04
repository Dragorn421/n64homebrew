#include <libdragon.h>

void function(void)
{
    static int count = 0;
    count++;
    fprintf(stderr, "%d\n", count);
}

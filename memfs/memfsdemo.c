#include <libdragon.h>

#include "memfs.h"

int main()
{
    debug_init_usblog();

    int i;

    i = memfs_init("mem:/");

    if (i == 0)
        debugf("attached mem:/ fs successfully\n");
    else
        debugf("failed to attach memfs: %d\n", i);

    struct memfs_buffer *memfs_buf = memfs_new_buf("bleh", 64);

    FILE *memfile = fopen("mem:/bleh", "r+");

    if (memfile != NULL)
        debugf("opened memfile successfully\n");
    else
        debugf("failed to open memfile\n");

    i = fwrite("abcdefgh", 1, 8, memfile);
    debugf("wrote %d\n", i);

    char buf[20];

    memset(buf, 0, sizeof(buf));
    fseek(memfile, 0, SEEK_SET);
    i = fread(buf, 1, 4, memfile);
    debugf("read %d %s\n", i, buf);

    debugf("ftell -> %ld\n", ftell(memfile));

    fseek(memfile, 4, SEEK_SET);
    i = fwrite("IJ", 1, 2, memfile);
    debugf("wrote %d\n", i);

    debugf("ftell -> %ld\n", ftell(memfile));

    memset(buf, 0, sizeof(buf));
    fseek(memfile, 0, SEEK_SET);
    i = fread(buf, 1, 15, memfile);
    debugf("read %d %s\n", i, buf);

    memfs_buf->buf[memfs_buf->buf_used] = '\0'; // hack
    debugf("memfs_buf->buf %s\n", memfs_buf->buf);

    while (true)
        ;
}
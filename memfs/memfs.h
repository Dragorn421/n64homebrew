#ifndef MEMFS_H
#define MEMFS_H

#include <stddef.h>
#include <stdint.h>

struct memfs_buffer
{
    char *name;
    uint8_t *buf;
    size_t buf_used;
    size_t buf_size;
    int n_handles;
};

struct memfs_buffer *memfs_new_buf(const char *name, size_t ini_size);
struct memfs_buffer *memfs_get_buf(const char *name);
bool memfs_delete_buf(struct memfs_buffer *buf);

/**
 * @return what libdragon's attach_filesystem returns
 */
int memfs_init(const char *prefix);

#endif

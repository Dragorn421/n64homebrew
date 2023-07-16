#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/param.h>

#include <system.h>
#include <debug.h>

#include "memfs.h"

// Note: this code isn't thread safe for several reasons

#define MAX_MEMFS_BUFS 5

struct memfs_buffer *bufs[MAX_MEMFS_BUFS];

struct memfs_buffer *memfs_new_buf(const char *name, size_t ini_size)
{
    int free_buf_i = -1;
    for (int i = 0; i < MAX_MEMFS_BUFS; i++)
    {
        if (bufs[i] == NULL)
        {
            free_buf_i = i;
            break;
        }
    }
    if (free_buf_i < 0)
    {
        return NULL;
    }

    struct memfs_buffer *buf = malloc(sizeof(struct memfs_buffer));
    if (buf == NULL)
    {
        return NULL;
    }

    buf->name = strdup(name);
    if (buf->name == NULL)
    {
        free(buf);
        return NULL;
    }

    buf->buf_size = ini_size;
    buf->buf = malloc(buf->buf_size);
    if (buf->buf == NULL)
    {
        free(buf->name);
        free(buf);
        return NULL;
    }

    buf->buf_used = 0;
    buf->n_handles = 0;

    bufs[free_buf_i] = buf;

    return buf;
}

struct memfs_buffer *memfs_get_buf(const char *name)
{
    for (int i = 0; i < MAX_MEMFS_BUFS; i++)
    {
        if (bufs[i] != NULL)
        {
            if (!strcmp(bufs[i]->name, name))
            {
                return bufs[i];
            }
        }
    }
    return NULL;
}

bool memfs_delete_buf(struct memfs_buffer *buf)
{
    if (buf->n_handles != 0)
    {
        return false;
    }

    for (int i = 0; i < MAX_MEMFS_BUFS; i++)
    {
        if (bufs[i] == buf)
        {
            bufs[i] = NULL;
        }
    }
    free(buf->name);
    free(buf->buf);
    free(buf);
    return true;
}

struct memfs_handle
{
    struct memfs_buffer *buf;
    size_t cur_pos;
};

void *memfs_open(char *name, int flags)
{
    struct memfs_buffer *buf = memfs_get_buf(name);

    if (buf == NULL)
    {
        return NULL;
    }

    struct memfs_handle *handle;

    handle = malloc(sizeof(struct memfs_handle));
    if (handle == NULL)
    {
        return NULL;
    }

    buf->n_handles += 1;

    handle->buf = buf;
    handle->cur_pos = 0;

    return handle;
}

int memfs_close(void *file)
{
    struct memfs_handle *handle = file;
    handle->buf->n_handles -= 1;
    free(handle);
    return 0;
}

int memfs_read(void *file, uint8_t *ptr, int len)
{
    struct memfs_handle *handle = file;

    size_t n_avail = handle->buf->buf_used - handle->cur_pos;
    if (len > n_avail)
    {
        len = n_avail;
    }

    if (len != 0)
    {
        memcpy(ptr, &handle->buf->buf[handle->cur_pos], len);
        handle->cur_pos += len;
    }

    return len;
}

int memfs_write(void *file, uint8_t *ptr, int len)
{
    struct memfs_handle *handle = file;

    size_t required_size = handle->cur_pos + len;

    if (required_size > handle->buf->buf_size)
    {
        size_t new_buf_size = required_size + 4 * 1024;
        void *realloc_buf = realloc(handle->buf->buf, new_buf_size);
        if (realloc_buf != NULL)
        {
            handle->buf->buf = realloc_buf;
            handle->buf->buf_size = new_buf_size;
        }
    }

    if (required_size > handle->buf->buf_size)
    {
        len = handle->buf->buf_size - handle->cur_pos;
    }

    if (len != 0)
    {
        memcpy(&handle->buf->buf[handle->cur_pos], ptr, len);
        handle->cur_pos += len;
        handle->buf->buf_used = MAX(handle->buf->buf_used, handle->cur_pos);
    }

    return len;
};

int memfs_lseek(void *file, int ptr, int dir)
{
    struct memfs_handle *handle = file;

    size_t new_pos = handle->cur_pos;

    switch (dir)
    {
    case SEEK_SET:
        new_pos = ptr;
        break;
    case SEEK_CUR:
        new_pos = handle->cur_pos + ptr;
        break;
    case SEEK_END:
        new_pos = handle->buf->buf_used + ptr;
        break;
    }

    if (new_pos >= 0 && new_pos <= handle->buf->buf_used)
    {
        handle->cur_pos = new_pos;
    }

    debugf("<memfs_lseek cur_pos=0x%X\n", handle->cur_pos);

    return handle->cur_pos;
}

filesystem_t mem_filesystem = {
    .open = memfs_open,
    .close = memfs_close,
    .read = memfs_read,
    .write = memfs_write,
    .lseek = memfs_lseek,
};

int memfs_init(const char *prefix)
{
    return attach_filesystem(prefix, &mem_filesystem);
}

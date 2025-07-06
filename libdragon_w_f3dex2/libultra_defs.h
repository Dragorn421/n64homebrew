#ifndef LIBULTRA_DEFS_H
#define LIBULTRA_DEFS_H

#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;

#define OS_TASK_YIELDED       0x0001
#define OS_TASK_LOADABLE      0x0004

#define OS_YIELD_DATA_SIZE 0xC00

typedef struct OSTask_t {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 flags;

    /* 0x08 */ u64* ucode_boot;
    /* 0x0C */ u32 ucode_boot_size;

    /* 0x10 */ u64* ucode;
    /* 0x14 */ u32 ucode_size;

    /* 0x18 */ u64* ucode_data;
    /* 0x1C */ u32 ucode_data_size;

    /* 0x20 */ u64* dram_stack;
    /* 0x24 */ u32 dram_stack_size;

    /* 0x28 */ u64* output_buff;
    /* 0x2C */ u64* output_buff_size;

    /* 0x30 */ u64* data_ptr;
    /* 0x34 */ u32 data_size;

    /* 0x38 */ u64* yield_data_ptr;
    /* 0x3C */ u32 yield_data_size;
} OSTask_t; // size = 0x40

typedef union OSTask {
    OSTask_t t;
    long long int force_structure_alignment;
} OSTask;

#define SP_DRAM_STACK_SIZE8     (0x400)
#define SP_DRAM_STACK_SIZE64    (SP_DRAM_STACK_SIZE8 >> 3)

#define SP_UCODE_SIZE           0x1000

#define SP_UCODE_DATA_SIZE      0x800

#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <libdragon.h>

#include "/home/dragorn421/Documents/oot/include/ultra64/ucode.h"
#include "/home/dragorn421/Documents/oot/include/ultra64/sptask.h"
#include "/home/dragorn421/Documents/oot/include/ultra64/gbi.h"

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ALIGNED(n) __attribute__((aligned(n)))

ALIGNED(16)
u64 gGfxSPTaskOutputBuffer[0x3000];

ALIGNED(16)
u64 gGfxSPTaskYieldBuffer[OS_YIELD_DATA_SIZE / sizeof(u64)];

ALIGNED(16)
u64 gGfxSPTaskStack[SP_DRAM_STACK_SIZE64];

ALIGNED(16)
Gfx workBuffer[1000];

//
// /home/dragorn421/Documents/oot/src/code/sys_ucode.c
u64 *sDefaultGSPUCodeText = gspF3DZEX2_NoN_PosLight_fifoTextStart;
u64 *sDefaultGSPUCodeData = gspF3DZEX2_NoN_PosLight_fifoDataStart;

u64 *SysUcode_GetUCodeBoot(void)
{
    return rspbootTextStart;
}

size_t SysUcode_GetUCodeBootSize(void)
{
    return (size_t)((u8 *)rspbootTextEnd - (u8 *)rspbootTextStart);
}

u64 *SysUcode_GetUCode(void)
{
    return sDefaultGSPUCodeText;
}

u64 *SysUcode_GetUCodeData(void)
{
    return sDefaultGSPUCodeData;
}
//

u32 __osSpDeviceBusy(void)
{
    if (*SP_STATUS & (SP_STATUS_DMA_BUSY | SP_STATUS_DMA_FULL | SP_STATUS_IO_BUSY))
    {
        return 1;
    }
    return 0;
}

s32 __osSpSetPc(void *pc)
{
    if (!(*SP_STATUS & SP_STATUS_HALTED))
    {
        return -1;
    }

    *SP_PC = (uint32_t)pc;
    return 0;
}

static OSTask sTmpTask;

OSTask *_VirtualToPhysicalTask(OSTask *intp)
{
    OSTask *tp = &sTmpTask;

    memcpy(tp, intp, sizeof(OSTask));

#define _osVirtualToPhysical(ptr)                                       \
    if (ptr != NULL)                                                    \
    {                                                                   \
        /* compared to libultra this doesn't handle TLB but whatever */ \
        ptr = (void *)PhysicalAddr(ptr);                                \
    }

    _osVirtualToPhysical(tp->t.ucode);
    _osVirtualToPhysical(tp->t.ucode_data);
    _osVirtualToPhysical(tp->t.dram_stack);
    _osVirtualToPhysical(tp->t.output_buff);
    _osVirtualToPhysical(tp->t.output_buff_size);
    _osVirtualToPhysical(tp->t.data_ptr);
    _osVirtualToPhysical(tp->t.yield_data_ptr);

    return tp;
}

#define OS_READ 0  // device -> RDRAM
#define OS_WRITE 1 // device <- RDRAM
s32 __osSpRawStartDma(s32 direction, void *devAddr, void *dramAddr, u32 size)
{
    if (__osSpDeviceBusy())
    {
        return -1;
    }

#define SP_BASE_REG 0xA4040000
#define SP_MEM_ADDR_REG (volatile uint32_t *)(SP_BASE_REG + 0x00)
#define SP_DRAM_ADDR_REG (volatile uint32_t *)(SP_BASE_REG + 0x04)
#define SP_RD_LEN_REG (volatile uint32_t *)(SP_BASE_REG + 0x08)
#define SP_WR_LEN_REG (volatile uint32_t *)(SP_BASE_REG + 0x0C)

    *SP_MEM_ADDR_REG = (uint32_t)devAddr;
    *SP_DRAM_ADDR_REG = PhysicalAddr(dramAddr);
    if (direction == OS_READ)
    {
        *SP_WR_LEN_REG = size - 1;
    }
    else
    {
        *SP_RD_LEN_REG = size - 1;
    }
    return 0;
}

void osSpTaskLoad(OSTask *intp)
{
    OSTask *tp = _VirtualToPhysicalTask(intp);

    if (tp->t.flags & OS_TASK_YIELDED)
    {
        tp->t.ucode_data = tp->t.yield_data_ptr;
        tp->t.ucode_data_size = tp->t.yield_data_size;
        intp->t.flags &= ~OS_TASK_YIELDED;

        if (tp->t.flags & OS_TASK_LOADABLE)
        {
            assert(false);
            // tp->t.ucode = (u64 *)IO_READ((u32)intp->t.yield_data_ptr + OS_YIELD_DATA_SIZE - 4);
        }
    }
    data_cache_hit_writeback(tp, sizeof(OSTask));
    *SP_STATUS = SP_WSTATUS_CLEAR_SIG0 | SP_WSTATUS_CLEAR_SIG1 | SP_WSTATUS_CLEAR_SIG2 | SP_WSTATUS_SET_INTR_BREAK;

    while (__osSpSetPc((void *)SP_IMEM) == -1)
    {
        ;
    }
#define SP_DMEM_END ((uint32_t)SP_DMEM + 0x1000 - 1)
    // could use libdragon's rsp_load_data ?
    while (__osSpRawStartDma(OS_WRITE, (void *)(SP_DMEM_END + 1 - sizeof(OSTask)), tp, sizeof(OSTask)) == -1)
    {
        ;
    }
    while (__osSpDeviceBusy())
    {
        ;
    }
    while (__osSpRawStartDma(OS_WRITE, (void *)SP_IMEM, tp->t.ucode_boot, tp->t.ucode_boot_size) == -1)
    {
        ;
    }
}

void osSpTaskStartGo(OSTask *tp)
{
    while (__osSpDeviceBusy())
    {
        ;
    }
    *SP_STATUS = SP_WSTATUS_SET_INTR_BREAK | SP_WSTATUS_CLEAR_SSTEP | SP_WSTATUS_CLEAR_BROKE | SP_WSTATUS_CLEAR_HALT;
}

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
ALIGNED(64)
u16 colorbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
ALIGNED(64)
u16 zbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void my_SP_interrupt_handler(void)
{
    fprintf(stderr, "my_SP_interrupt_handler\n");
}

void my_DP_interrupt_handler(void)
{
    fprintf(stderr, "my_DP_interrupt_handler\n");
}

int main()
{
    debug_init_isviewer();
    fprintf(stderr, "hey\n");

    OSTask taskData;
    OSTask_t *task = &taskData.t;

    Gfx *workBufferEnd = workBuffer;

    register_SP_handler(my_SP_interrupt_handler);
    register_DP_handler(my_DP_interrupt_handler);

    // Gfx_SetupFrame
    gDPSetScissor(workBufferEnd++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetColorImage(workBufferEnd++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, colorbuffer);
    gDPSetDepthImage(workBufferEnd++, zbuffer);

    gDPSetCycleType(workBufferEnd++, G_CYC_FILL);
    gDPSetRenderMode(workBufferEnd++, G_RM_NOOP, G_RM_NOOP2);
    u8 r = 255, g = 255, b = 0;
    gDPSetFillColor(workBufferEnd++, (GPACK_RGBA5551(r, g, b, 1) << 16) | GPACK_RGBA5551(r, g, b, 1));
    gDPFillRectangle(workBufferEnd++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    gDPPipeSync(workBufferEnd++);

    // Graph_Update
    gDPFullSync(workBufferEnd++);

    gSPEndDisplayList(workBufferEnd++); // ?

    task->type = M_GFXTASK;
    task->flags = 0x0004; // OS_SC_DRAM_DLIST, no effect
    task->ucode_boot = SysUcode_GetUCodeBoot();
    task->ucode_boot_size = SysUcode_GetUCodeBootSize();
    task->ucode = SysUcode_GetUCode();
    task->ucode_data = SysUcode_GetUCodeData();
    task->ucode_size = SP_UCODE_SIZE;
    task->ucode_data_size = SP_UCODE_DATA_SIZE;
    task->dram_stack = gGfxSPTaskStack;
    task->dram_stack_size = sizeof(gGfxSPTaskStack);
    task->output_buff = gGfxSPTaskOutputBuffer;
    task->output_buff_size = gGfxSPTaskOutputBuffer + ARRAY_COUNT(gGfxSPTaskOutputBuffer);
    task->data_ptr = (u64 *)workBuffer;

    task->data_size = (uintptr_t)workBufferEnd - (uintptr_t)workBuffer;

    task->yield_data_ptr = gGfxSPTaskYieldBuffer;

    task->yield_data_size = sizeof(gGfxSPTaskYieldBuffer);

    data_cache_writeback_invalidate_all();

    osSpTaskLoad(&taskData);
    osSpTaskStartGo(&taskData);

    fprintf(stderr, "main done\n");
    for (int i = 0; i < 20; i++)
    {
        fprintf(stderr, "%08lX %08lX %04hX\n", *SP_PC, *SP_STATUS, *UncachedShortAddr(&colorbuffer[0]));
        wait_ms(10);
    }
    return 0;
}

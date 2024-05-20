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

void run_rsp_task(OSTask *intp)
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

    assert(*SP_STATUS & SP_STATUS_HALTED);
    *SP_PC = 0;
    rsp_load_data(tp, sizeof(OSTask), 0x1000 - sizeof(OSTask));
    rsp_load_code(tp->t.ucode_boot, tp->t.ucode_boot_size, 0);
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
    set_SP_interrupt(1);
    set_DP_interrupt(1);

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

    gSPEndDisplayList(workBufferEnd++);

    task->type = M_GFXTASK;
    task->flags = OS_TASK_LOADABLE;
    task->ucode_boot = rspbootTextStart;
    task->ucode_boot_size = (uint32_t)rspbootTextEnd - (uint32_t)rspbootTextStart;
    task->ucode = gspF3DZEX2_NoN_PosLight_fifoTextStart;
    task->ucode_data = gspF3DZEX2_NoN_PosLight_fifoDataStart;
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

    run_rsp_task(&taskData);

    fprintf(stderr, "main done\n");
    for (int i = 0; i < 5; i++)
    {
        fprintf(stderr, "%08lX %08lX %04hX\n", *SP_PC, *SP_STATUS, *UncachedShortAddr(&colorbuffer[0]));
        wait_ms(10);
    }
    return 0;
}

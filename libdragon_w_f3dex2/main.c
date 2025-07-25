#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <libdragon.h>

#include "libultra_defs.h"

#define F3DEX_GBI_2
#include "/home/dragorn421/Documents/oot/include/ultra64/gbi.h"

extern uint64_t gspF3DZEX2_NoN_PosLight_fifoTextStart[];
extern uint64_t gspF3DZEX2_NoN_PosLight_fifoDataStart[];

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ALIGNED(n) __attribute__((aligned(n)))

ALIGNED(16)
uint64_t gGfxSPTaskOutputBuffer[0x3000];

ALIGNED(16)
uint64_t gGfxSPTaskYieldBuffer[OS_YIELD_DATA_SIZE / sizeof(uint64_t)];

ALIGNED(16)
uint64_t gGfxSPTaskStack[SP_DRAM_STACK_SIZE64];

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
    *SP_PC = 0x80;
    assert(sizeof(OSTask) == 0x40);
    assert(tp->t.ucode_data_size <= 0xFC0);
    rsp_load_data(tp->t.ucode_data, tp->t.ucode_data_size, 0);
    rsp_load_data(tp, sizeof(OSTask), 0xFC0);
    // rsp_load_code(tp->t.ucode_boot, 0x80, 0); // seems useless
    rsp_load_code(tp->t.ucode, 0xF80, 0x80);
    *SP_STATUS = SP_WSTATUS_SET_INTR_BREAK | SP_WSTATUS_CLEAR_SSTEP | SP_WSTATUS_CLEAR_BROKE | SP_WSTATUS_CLEAR_HALT;
}

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
ALIGNED(64)
u16 zbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

bool seen_sp_intr = false;
bool seen_dp_intr = false;

void my_SP_interrupt_handler(void)
{
    fprintf(stderr, "my_SP_interrupt_handler\n");
    seen_sp_intr = true;
}

void my_DP_interrupt_handler(void)
{
    fprintf(stderr, "my_DP_interrupt_handler\n");
    seen_dp_intr = true;
}

void wait_rsp_task(void)
{
    RSP_WAIT_LOOP(200)
    {
        disable_interrupts();
        bool seen_sp_and_dp_intr = seen_sp_intr && seen_dp_intr;
        if (seen_sp_and_dp_intr)
            seen_sp_intr = seen_dp_intr = false;
        enable_interrupts();
        if (seen_sp_and_dp_intr)
            break;
    }
}

int main()
{
    debug_init_isviewer();
    set_SP_interrupt(1);
    set_DP_interrupt(1);

    register_SP_handler(my_SP_interrupt_handler);
    register_DP_handler(my_DP_interrupt_handler);

    fprintf(stderr, "hey\n");

    display_init((resolution_t){.width = SCREEN_WIDTH, .height = SCREEN_HEIGHT, .interlaced = false},
                 DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);

    while (true)
    {
        surface_t *surf = display_get();

        OSTask taskData;
        OSTask_t *task = &taskData.t;

        Gfx *workBufferEnd = workBuffer;

        // Gfx_SetupFrame
        gDPSetScissor(workBufferEnd++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        gDPSetColorImage(workBufferEnd++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, surf->buffer);
        gDPSetDepthImage(workBufferEnd++, zbuffer);

        gDPSetCycleType(workBufferEnd++, G_CYC_FILL);
        gDPSetRenderMode(workBufferEnd++, G_RM_NOOP, G_RM_NOOP2);
        u8 r = abs((get_ticks_ms() / 5) % 200 - 100), g = 100, b = 50;
        gDPSetFillColor(workBufferEnd++, (GPACK_RGBA5551(r, g, b, 1) << 16) | GPACK_RGBA5551(r, g, b, 1));
        gDPFillRectangle(workBufferEnd++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        gDPPipeSync(workBufferEnd++);

        // Graph_Update
        gDPFullSync(workBufferEnd++);

        gSPEndDisplayList(workBufferEnd++);

        task->type = 1; // M_GFXTASK
        task->flags = OS_TASK_LOADABLE;
        // task->ucode_boot = rspbootTextStart;
        // task->ucode_boot_size = (uint32_t)rspbootTextEnd - (uint32_t)rspbootTextStart;
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

        wait_rsp_task();

        display_show(surf);
    }
    return 0;
}

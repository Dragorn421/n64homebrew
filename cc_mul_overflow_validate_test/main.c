#include <libdragon.h>

int main()
{
    debug_init_isviewer();

    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_DISABLED);

    rdpq_init();

    rdpq_debug_start();

    rdpq_debug_log(true);

    surface_t *surf = display_get();
    rdpq_attach(surf, NULL);

    rdpq_clear((color_t){50, 100, 50, 0xFF});

    rdpq_set_mode_standard();
    rdpq_set_prim_color((color_t){0xFF, 0x80, 0x40, 0xFF});

    int posX = 10, posY = 10;

    rdpq_debug_log_msg("genuine");
    rdpq_mode_combiner(RDPQ_COMBINER2(
        (0, 0, 0, 1), (0, 0, 0, 1),
        (PRIM, 0, PRIM_ALPHA, 0), (0, 0, 0, 1)));
    rdpq_fill_rectangle(posX, posY, posX + 32, posY + 32);

    rdpq_debug_log_msg("0 0 0 1 -> mul=COMBINED");
    posX += 40;
    rdpq_mode_combiner(RDPQ_COMBINER2(
        (0, 0, 0, 1), (0, 0, 0, 1),
        (PRIM, 0, COMBINED, 0), (0, 0, 0, 1)));
    rdpq_fill_rectangle(posX, posY, posX + 32, posY + 32);

    rdpq_debug_log_msg("0 0 0 1 -> suba=COMBINED");
    posX += 40;
    rdpq_mode_combiner(RDPQ_COMBINER2(
        (0, 0, 0, 1), (0, 0, 0, 1),
        (COMBINED, 0, PRIM, 0), (0, 0, 0, 1)));
    rdpq_fill_rectangle(posX, posY, posX + 32, posY + 32);

    rdpq_debug_log_msg("1 0 PRIM 0 -> mul=COMBINED");
    posX += 40;
    rdpq_mode_combiner(RDPQ_COMBINER2(
        (1, 0, PRIM, 0), (0, 0, 0, 1),
        (PRIM, 0, COMBINED, 0), (0, 0, 0, 1)));
    rdpq_fill_rectangle(posX, posY, posX + 32, posY + 32);

    rdpq_detach_show();

    while (true)
    {
    }
}

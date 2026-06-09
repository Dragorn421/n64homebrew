#include <stdint.h>

#include <libdragon.h>

int main() {
    debug_init_emulog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);
    rdpq_init();
    rdpq_debug_start();
    rdpq_debug_log(true);

    uint16_t tex_buffer[32][32];
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            tex_buffer[i][j] = abs(i - j) < 3 ? 0x0000 : 0xFFFF;
        }
    }
    surface_t tex = surface_make_linear(tex_buffer, FMT_RGBA16, 32, 32);

    data_cache_writeback_invalidate_all();

    while (true) {
        surface_t *surf = display_get();
        rdpq_attach(surf, NULL);
        rdpq_clear((color_t){100, 100, 0, 255});
        rdpq_set_mode_standard();
        rdpq_mode_combiner(RDPQ_COMBINER_TEX);
        rdpq_tex_upload(TILE0, &tex, &(rdpq_texparms_t){});

        rdp_draw_textured_rectangle(TILE0, 10, 10, 42, 42, MIRROR_DISABLED);
        float v1[] = {50.0f, 10.0f, 0.0f, 0.0f, 1.0f};
        float v2[] = {50.0f, 50.0f, 0.0f, 32.0f, 1.0f};
        float v3[] = {90.0f, 10.0f, 32.0f, 0.0f, 1.0f};
        float v4[] = {90.0f, 50.0f, 32.0f, 32.0f, 1.0f};
        rdpq_triangle(&TRIFMT_TEX, v1, v2, v3);
        rdpq_triangle(&TRIFMT_TEX, v4, v2, v3);

        rdpq_mode_alphacompare(120);

        rdp_draw_textured_rectangle(TILE0, 10, 60, 42, 92, MIRROR_DISABLED);
        float v1b[] = {50.0f, 60.0f, 0.0f, 0.0f, 1.0f};
        float v2b[] = {50.0f, 100.0f, 0.0f, 32.0f, 1.0f};
        float v3b[] = {90.0f, 60.0f, 32.0f, 0.0f, 1.0f};
        float v4b[] = {90.0f, 100.0f, 32.0f, 32.0f, 1.0f};
        rdpq_triangle(&TRIFMT_TEX, v1b, v2b, v3b);
        rdpq_triangle(&TRIFMT_TEX, v4b, v2b, v3b);

        rdpq_mode_antialias(AA_STANDARD);

        rdp_draw_textured_rectangle(TILE0, 10, 110, 42, 142, MIRROR_DISABLED);
        float v1c[] = {50.0f, 110.0f, 0.0f, 0.0f, 1.0f};
        float v2c[] = {50.0f, 150.0f, 0.0f, 32.0f, 1.0f};
        float v3c[] = {90.0f, 110.0f, 32.0f, 0.0f, 1.0f};
        float v4c[] = {90.0f, 150.0f, 32.0f, 32.0f, 1.0f};
        rdpq_triangle(&TRIFMT_TEX, v1c, v2c, v3c);
        rdpq_triangle(&TRIFMT_TEX, v4c, v2c, v3c);

        rdpq_detach_show();
    }
}

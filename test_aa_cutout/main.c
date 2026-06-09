#include <stdint.h>

#include <libdragon.h>

int main() {
    debug_init_emulog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);
    rdpq_init();
    rdpq_debug_start();
    rdpq_debug_log(true);

    uint16_t tlut[2] = {
        0x0000, // transparent black
        0xFFFF, // opaque white
    };
    // generate a texture with a transparent black diagonal and white elsewhere
    uint8_t tex_buffer[32][32];
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            tex_buffer[i][j] = abs(i - j) < 3 ? 0 : 1;
        }
    }
    surface_t tex = surface_make_linear(tex_buffer, FMT_CI8, 32, 32);

    data_cache_writeback_invalidate_all();

    while (true) {
        surface_t *surf = display_get();
        rdpq_attach(surf, NULL);
        rdpq_clear((color_t){100, 100, 0, 255});

        rdpq_set_mode_standard();
        rdpq_mode_combiner(RDPQ_COMBINER_TEX);
        rdpq_mode_tlut(TLUT_RGBA16);
        rdpq_tex_upload(TILE0, &tex, &(rdpq_texparms_t){});
        rdpq_tex_upload_tlut(tlut, 0, 2);

        // No AA nor alpha compare

        rdpq_texture_rectangle(TILE0, 10, 10, 42, 42, 0, 0);
        float v1[] = {50.0f, 10.0f, 0.0f, 0.0f, 1.0f};
        float v2[] = {50.0f, 50.0f, 0.0f, 32.0f, 1.0f};
        float v3[] = {90.0f, 10.0f, 32.0f, 0.0f, 1.0f};
        float v4[] = {90.0f, 50.0f, 32.0f, 32.0f, 1.0f};
        rdpq_triangle(&TRIFMT_TEX, v1, v2, v3);
        rdpq_triangle(&TRIFMT_TEX, v4, v2, v3);

        // No AA, alpha compare

        rdpq_mode_alphacompare(120);

        rdpq_texture_rectangle(TILE0, 10, 60, 42, 92, 0, 0);
        float v1b[] = {50.0f, 60.0f, 0.0f, 0.0f, 1.0f};
        float v2b[] = {50.0f, 100.0f, 0.0f, 32.0f, 1.0f};
        float v3b[] = {90.0f, 60.0f, 32.0f, 0.0f, 1.0f};
        float v4b[] = {90.0f, 100.0f, 32.0f, 32.0f, 1.0f};
        rdpq_triangle(&TRIFMT_TEX, v1b, v2b, v3b);
        rdpq_triangle(&TRIFMT_TEX, v4b, v2b, v3b);

        // AA and alpha compare

        rdpq_mode_antialias(AA_STANDARD);

        rdpq_texture_rectangle(TILE0, 10, 110, 42, 142, 0, 0);
        float v1c[] = {50.0f, 110.0f, 0.0f, 0.0f, 1.0f};
        float v2c[] = {50.0f, 150.0f, 0.0f, 32.0f, 1.0f};
        float v3c[] = {90.0f, 110.0f, 32.0f, 0.0f, 1.0f};
        float v4c[] = {90.0f, 150.0f, 32.0f, 32.0f, 1.0f};
        rdpq_triangle(&TRIFMT_TEX, v1c, v2c, v3c);
        rdpq_triangle(&TRIFMT_TEX, v4c, v2c, v3c);

        // Still AA and alpha compare but the rdpq_mode_tlut call breaks alpha
        // compare somehow

        rdpq_mode_tlut(TLUT_RGBA16);

        rdpq_texture_rectangle(TILE0, 10, 160, 42, 192, 0, 0);
        float v1d[] = {50.0f, 160.0f, 0.0f, 0.0f, 1.0f};
        float v2d[] = {50.0f, 200.0f, 0.0f, 32.0f, 1.0f};
        float v3d[] = {90.0f, 160.0f, 32.0f, 0.0f, 1.0f};
        float v4d[] = {90.0f, 200.0f, 32.0f, 32.0f, 1.0f};
        rdpq_triangle(&TRIFMT_TEX, v1d, v2d, v3d);
        rdpq_triangle(&TRIFMT_TEX, v4d, v2d, v3d);

        rdpq_detach_show();
    }
}

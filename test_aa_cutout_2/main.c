#include <stdint.h>

#include <libdragon.h>

int main() {
    debug_init_emulog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);
    rdpq_init();
    if (0) {
        rdpq_debug_start();
        rdpq_debug_log(true);
    }

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

    joypad_init();

    while (true) {
        joypad_poll();
        joypad_buttons_t buttons_held = joypad_get_buttons_held(JOYPAD_PORT_1);
        rdpq_attach(display_get(), NULL);
        rdpq_clear((color_t){100, 0, 0, 255});
        rdpq_set_mode_standard();
        rdpq_set_prim_color((color_t){0, 100, 0, 255});
        rdpq_mode_tlut(TLUT_RGBA16);
        rdpq_mode_antialias(AA_STANDARD);
        rdpq_tex_upload(TILE0, &tex, &(rdpq_texparms_t){});
        rdpq_tex_upload_tlut(tlut, 0, 2);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (i == 1 && j == 1)
                    continue;
                rdpq_mode_alphacompare(120);
                if (i == 0) {
                    rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
                    if (j == 1)
                        rdpq_mode_alphacompare(0);
                } else {
                    rdpq_mode_combiner(RDPQ_COMBINER_TEX_FLAT);
                }
                float v1[] = {10.0f + j * 160.0f, 10.0f + i * 120.0f, 0.0f,
                              0.0f, 1.0f};
                float v2[] = {10.0f + j * 160.0f, 110.0f + i * 120.0f, 0.0f,
                              32.0f, 1.0f};
                float v3[] = {150.0f + j * 160.0f, 10.0f + i * 120.0f, 32.0f,
                              0.0f, 1.0f};
                float v4[] = {150.0f + j * 160.0f, 110.0f + i * 120.0f, 32.0f,
                              32.0f, 1.0f};
                rdpq_triangle(&TRIFMT_TEX, v1, v2, v3);
                rdpq_triangle(&TRIFMT_TEX, v4, v2, v3);
            }
        }

        rdpq_mode_combiner(RDPQ_COMBINER_TEX);
        rdpq_mode_alphacompare(120);
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                float v1[] = {40.0f + i * 160.0f, 40.0f + j * 120.0f, 0.0f,
                              0.0f, 1.0f};
                float v2[] = {80.0f + i * 160.0f, 100.0f + j * 120.0f, 0.0f,
                              32.0f, 1.0f};
                float v3[] = {70.0f + i * 160.0f, 30.0f + j * 120.0f, 32.0f,
                              0.0f, 1.0f};
                float v4[] = {120.0f + i * 160.0f, 90.0f + j * 120.0f, 32.0f,
                              32.0f, 1.0f};
                rdpq_triangle(&TRIFMT_TEX, v1, v2, v3);
                rdpq_triangle(&TRIFMT_TEX, v4, v2, v3);
            }
        }

        rdpq_detach_show();
    }
}

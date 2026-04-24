#include <libdragon.h>

int main()
{
    debug_init_isviewer();

    joypad_init();

    console_init();

    rdpq_init();

    rdpq_debug_start();
    rdpq_debug_log(true);

#define TEX_FMT FMT_RGBA16
#define TEX_WIDTH 16
#define TEX_HEIGHT 4
    surface_t tex_surf = surface_alloc(TEX_FMT, TEX_WIDTH, TEX_HEIGHT);
    surface_t fb_surf = surface_alloc(FMT_RGBA16, 32, 8);

    rdpq_set_texture_image(&tex_surf);

    printf("Instructions:\n");
    printf("Press A to make rdpq load a texture, "
           "render it to an offscreen surface, "
           "and print the result to stdout.\n");
    printf("The tricky part for emulators is that the address "
           "of the texture to be loaded is not set when pressing A, "
           "but instead only set once when the rom starts up. "
           "(rdpq_set_texture_image)\n");
    printf("To test if savestating preserves set_texture_image, "
           "make a savestate now, load it, then press A, "
           "and see if the rom crashes.\n");

    console_set_render_mode(RENDER_MANUAL);

    while (true)
    {
        joypad_poll();
        if (joypad_get_buttons_pressed(JOYPAD_PORT_1).a)
        {
            rdpq_attach(&tex_surf, NULL);
            rdpq_clear((color_t){0, 255, 0, 255});
            rdpq_detach();

            rdpq_attach(&fb_surf, NULL);
            rdpq_clear((color_t){255, 0, 0, 255});
            rdpq_set_tile(TILE0, TEX_FMT, 0, 0, NULL);
            rdpq_load_block(TILE0, 0, 0, TEX_WIDTH * TEX_HEIGHT, 2 * TEX_WIDTH);
            rdpq_set_mode_standard();
            rdpq_set_mode_copy(false);
            rdpq_set_tile_size(TILE0, 0, 0, TEX_WIDTH, TEX_HEIGHT);
            rdpq_texture_rectangle(TILE0, 0, 0, TEX_WIDTH, TEX_HEIGHT, 0, 0);

            rdpq_detach_wait();

            // from src/libgbd/vt.h in Thar0/gbd
#define VT_ESC "\x1b"
#define VT_CSI "["
#define VT_SGR(n) VT_ESC VT_CSI n "m"
#define VT_RST VT_SGR("")
#define VT_RGBCOL_S(sb, sf) \
    VT_SGR("48;2;" sb)      \
    VT_SGR("38;2;" sf)
#define PRINT_PX(r, g, b) printf(VT_RGBCOL_S("%d;%d;%d", "%d;%d;%d") "\u2584\u2584", r, g, b, r, g, b)
            for (int y = 0; y < fb_surf.height; y++)
            {
                uint16_t *pixels = (uint16_t *)((char *)fb_surf.buffer + y * fb_surf.stride);
                for (int x = 0; x < fb_surf.width; x++)
                {
                    uint16_t px = pixels[x];
                    int r = (px & 0xF800) >> 11;
                    int g = (px & 0x07C0) >> 6;
                    int b = (px & 0x003E) >> 1;
                    r = (r << 3) | (r >> 2);
                    g = (g << 3) | (g >> 2);
                    b = (b << 3) | (b >> 2);
                    PRINT_PX(r, g, b);
                }
                printf(VT_RST "\n");
            }
        }
    }
}

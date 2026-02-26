#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

surface_t zbuffer;

int main()
{
    debug_init_isviewer();
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    rdpq_matdb_t *mdb = rdpq_matdb_open("rom:/materials.mdb");
    rdpq_mat_set_texture_path("rom:/texdb");
    rdpq_mat_t *mat_prim = rdpq_matdb_load(mdb, "prim");
    rdpq_mat_t *mat_textured = rdpq_matdb_load(mdb, "textured");

    joypad_init();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    rdpq_init();

#define RDPQ_DEBUG_ONE_FRAME false
    if (RDPQ_DEBUG_ONE_FRAME)
    {
        rdpq_debug_start();
        rdpq_debug_log(true);
#define RDPQ_LOG_FLAG_SHOWTRIS 0x00000001
        extern int __rdpq_debug_log_flags;
        __rdpq_debug_log_flags |= RDPQ_LOG_FLAG_SHOWTRIS;
    }

    gl_init();

    while (true)
    {
        surface_t *disp = display_get();

        joypad_poll();

        rdpq_mat_t *mat;

        if (joypad_get_buttons_held(JOYPAD_PORT_1).a)
        {
            mat = mat_prim;
        }
        else
        {
            mat = mat_textured;
        }

        rdpq_attach(disp, &zbuffer);

        gl_context_begin();

        glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_RDPQ_MATERIAL_N64);
        glEnable(GL_RDPQ_TEXTURING_N64);

        rdpq_mat_draw_begin(mat);

        rdpq_set_prim_color(RGBA32(255, 0, 0, 255));

        glTexSizeN64(32, 32);

        glBegin(GL_TRIANGLES);

        float f = 0.5f;
        glTexCoord2f(1, 1);
        glVertex3f(f, f, 0);
        glTexCoord2f(-3, 1);
        glVertex3f(-f, f, 0);
        glTexCoord2f(1, 4);
        glVertex3f(f, -f, 0);

        glEnd();

        rdpq_mat_draw_end(mat);

        glDisable(GL_RDPQ_MATERIAL_N64);
        glDisable(GL_RDPQ_TEXTURING_N64);

        gl_context_end();

        rdpq_detach();

        display_show(disp);

        if (RDPQ_DEBUG_ONE_FRAME)
        {
            while (1)
                ;
        }
    }
}

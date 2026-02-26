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
    rdpq_mat_t *mat = rdpq_matdb_load(mdb, "Material");

    joypad_init();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    rdpq_init();

    if (0)
    {
        rdpq_debug_start();
        rdpq_debug_log(true);
    }

    gl_init();

    while (true)
    {
        surface_t *disp = display_get();

        joypad_poll();

        if (joypad_get_buttons_pressed(JOYPAD_PORT_1).a)
        {
            fprintf(stderr, "Hey stderr\n");
        }

        rdpq_attach(disp, &zbuffer);

        gl_context_begin();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_RDPQ_MATERIAL_N64);

        rdpq_mat_draw_begin(mat);

        rdpq_set_prim_color(RGBA32(255, 0, 0, 255));

        glBegin(GL_TRIANGLES);

        float f = 0.5f;
        glVertex3f(f, f, 0);
        glVertex3f(-f, f, 0);
        glVertex3f(f, -f, 0);

        glEnd();

        rdpq_mat_draw_end(mat);

        glDisable(GL_RDPQ_MATERIAL_N64);

        gl_context_end();

        rdpq_detach();

        display_show(disp);
    }
}

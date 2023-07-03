#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <model64.h>

int main()
{
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();

    gl_init();

    surface_t zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    model64_t *model = model64_load("rom:/brewmodel.model64");

    while (true)
    {
        surface_t *disp = display_get();

        rdpq_attach(disp, &zbuffer);
        gl_context_begin();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_MULTISAMPLE_ARB);

        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(80.0f, 4.0f / 3.0f, 1.0f, 10.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            0, 3, -4,
            0, 0, 0,
            0, 1, 0);

        // model64_draw(model);
        glColor3f(1, 1, 0);
        model64_draw_mesh(model64_get_mesh(model, 0));
        glColor4f(1, 1, 1, 0.5);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        model64_draw_mesh(model64_get_mesh(model, 1));
        glDisable(GL_BLEND);

        gl_context_end();
        rdpq_detach_show();
    }
}

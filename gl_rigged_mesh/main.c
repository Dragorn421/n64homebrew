#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

#include "assets/handwritten.h"
#include "assets/cube_vertexcolored_customnormals_rigged.h"

int main()
{
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_CORRECT, ANTIALIAS_RESAMPLE_FETCH_NEEDED);
    surface_t zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    controller_init();

    rdpq_init();

    gl_init();

    while (true)
    {
        surface_t *surf = display_get();
        rdpq_attach(surf, &zbuffer);

        gl_context_begin();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // model_hourglass();

        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(50, 4.0f / 3, 0.1, 10);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(2, 4, 3,
                  0, 0, 1,
                  0, 0, 1);

        glEnable(GL_MATRIX_PALETTE_ARB);
        glMatrixMode(GL_MATRIX_PALETTE_ARB);

        glCurrentPaletteMatrixARB(0);
        glLoadIdentity();
        gluLookAt(2, 4, 3,
                  0, 0, 1,
                  0, 0, 1);

        glCurrentPaletteMatrixARB(1);
        glLoadIdentity();
        gluLookAt(2, 4, 3,
                  0, 0, 1,
                  0, 0, 1);
        glRotatef((float)TICKS_READ() / TICKS_PER_SECOND * 180, 0, 0, 1);

        model_Cube();

        gl_context_end();

        rdpq_detach_show();
    }
}

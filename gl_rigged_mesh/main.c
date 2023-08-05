#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

#include "assets/handwritten.h"
#include "assets/cube_vertexcolored_customnormals_rigged.h"
#include "assets/suzanne_rigged_normals.h"

int main()
{
    debug_init_isviewer();
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

        float viewYaw = (float)TICKS_READ() / TICKS_PER_SECOND * 360 / 10;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(1, -3, 1,
                  0, 0, 0,
                  0, 0, 1);
        glRotatef(viewYaw, 0, 0, 1);

        glEnable(GL_MATRIX_PALETTE_ARB);
        glMatrixMode(GL_MATRIX_PALETTE_ARB);
        for (int i = 0; i < 6; i++)
        {
            glCurrentPaletteMatrixARB(i);
            glLoadIdentity();
            gluLookAt(1, -3, 1,
                      0, 0, 0,
                      0, 0, 1);
            glRotatef(viewYaw, 0, 0, 1);
            /*
            glRotatef(
                20*cosf((float)TICKS_READ() / TICKS_PER_SECOND * 6.3),
                cos(i), cos(i * 3), cos(i * 7));
            */
            // debugf("%d %f\n", i, 20*cosf((float)TICKS_READ() / TICKS_PER_SECOND * 6.3));
        }
        /*
        0 Bone      chin
        1 Bone.001  "right"
        2 Bone.002  right ear
        3 Bone.003  "left"
        4 Bone.004  left ear
        5 Bone.005  "top"
        */
        float f = cosf((float)TICKS_READ() / TICKS_PER_SECOND * 6.3);

        glCurrentPaletteMatrixARB(0);
        glTranslatef(0, 0, 0.2 * (-1 + f));

        glCurrentPaletteMatrixARB(2);
        float s = 1 + 0.8 * (1 + f);
        glScalef(s, s, s);

        glCurrentPaletteMatrixARB(4);
        glTranslatef(0.2 * (1 + f), 0, 0);
        glRotatef(20 * f, 0, 1, 0);

        GLfloat lmodel_ambient[] = {0, 0, 0, 1.0};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glEnable(GL_LIGHTING);

        // Ambient: light vertices red regardless of their normals and the direction they're in
        glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat[4]){1, 0, 0, 1});
        // Diffuse: light vertices depending on the direction to the light lining up with the normals
        glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){0, 1, 0, 1});
        // positional light source (w != 0)
        glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[4]){2, -3, 2, 1});
        glEnable(GL_LIGHT0);

        glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat[4]){0, 0, 1, 1});
        glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat[4]){-2, 3, -2, 1});
        glEnable(GL_LIGHT1);

        model_Suzanne();

        if (0)
        {
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
        }

        gl_context_end();

        rdpq_detach_show();
    }
}

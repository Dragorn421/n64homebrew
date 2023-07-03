#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <model64.h>
#include <math.h>

// hand-written enum, guessed from trial and error
enum
{
    BREW3D_MESH_LIQUID,
    BREW3D_MESH_CUPBODY,
    BREW3D_MESH_HEAD,
    BREW3D_MESH_HANDLE,
    BREW3D_MESH_NLOGO
};

int main()
{
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();

    gl_init();

    surface_t zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    model64_t *model = model64_load("rom:/brewmodel.model64");

    debugf("model64_get_mesh_count(model) = %ld\n", model64_get_mesh_count(model));

    GLfloat angle = 0;

    // profiling
    int n_frames_store_starts = 50;
    int cur_frame_ring_i = 0;
    unsigned long frame_starts[n_frames_store_starts];
    unsigned long frame_ends[n_frames_store_starts];
    bool frame_starts_initialized = false;

    // use_the_dlist: true to generate a dlist for displaying the mesh and use it every frame
    bool use_the_dlist = false;
    GLuint the_dlist;
    bool the_dlist_is_built = false;

    while (true)
    {
        surface_t *disp = display_get();

        frame_starts[cur_frame_ring_i] = get_ticks();

        rdpq_attach(disp, &zbuffer);
        gl_context_begin();

        glClearColor(0.3, 0.2, 0.7, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_MULTISAMPLE_ARB);

        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(80.0f, 4.0f / 3.0f, 1.0f, 10.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            -5, 3, 1,
            0, 2, 0,
            0, 1, 0);

        // make the light turn around the object
        glPushMatrix();
        glRotatef(-angle * 3, 0, 1, 0);
        glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[4]){3, 3, 0, 1});
        glPopMatrix();

        // make the object rotate
        glRotatef(angle, 0, 1, 0);
        glRotatef(sinf(angle * 21) * 10, 1, 0, 0);
        angle += 360.0f / 100;

        if (!use_the_dlist || !the_dlist_is_built)
        {
            if (use_the_dlist)
            {
                the_dlist_is_built = true;

                the_dlist = glGenLists(1);
                glNewList(the_dlist, GL_COMPILE);
            }

            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat[4]){0.3, 0.3, 0.3, 1});
            glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){0.7, 0.7, 0.7, 1});

            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

            // glMaterialf(GL_FRONT_AND_BACK, GL_LINEAR_ATTENUATION, 1)

            assert(model64_get_mesh_count(model) == 5);

            glColor3f(0.982, 0.429, 0.033);
            model64_draw_mesh(model64_get_mesh(model, BREW3D_MESH_LIQUID));

            glColor4f(1, 1, 1, 0.196);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            model64_draw_mesh(model64_get_mesh(model, BREW3D_MESH_CUPBODY));
            model64_draw_mesh(model64_get_mesh(model, BREW3D_MESH_HANDLE));
            glDisable(GL_BLEND);

            glColor3f(1, 1, 1);
            model64_draw_mesh(model64_get_mesh(model, BREW3D_MESH_HEAD));
            model64_draw_mesh(model64_get_mesh(model, BREW3D_MESH_NLOGO));

            if (use_the_dlist)
            {
                glEndList();
            }
        }

        if (use_the_dlist)
        {
            glCallList(the_dlist);
        }

        gl_context_end();

        // wait for rdp to be done with the framebuffer to call graphics_draw_text (cpu framebuffer writing) below
        rdpq_detach_wait();

        frame_ends[cur_frame_ring_i] = get_ticks();
        cur_frame_ring_i++;
        if (cur_frame_ring_i >= n_frames_store_starts)
        {
            cur_frame_ring_i = 0;
            frame_starts_initialized = true;
        }
        if (frame_starts_initialized)
        {
            unsigned long average_processing_ticks_per_frame = 0;
            for (int i = 0; i < n_frames_store_starts; i++)
            {
                average_processing_ticks_per_frame += TICKS_DISTANCE(frame_starts[i], frame_ends[i]) / n_frames_store_starts;
            }
            float average_processing_ms_per_frame = (float)average_processing_ticks_per_frame / TICKS_PER_SECOND * 1000;
            char str[1000];
            snprintf(str, sizeof(str), "ms/frame=%f frame/s=%f",
                     average_processing_ms_per_frame, 1000 / average_processing_ms_per_frame);
            graphics_draw_text(disp, 10, 5, str);
        }

        display_show(disp);
    }
}

#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <model64.h>
#include <math.h>
#include <malloc.h>

uint32_t ROMHACKED_OFFSET = 0x104000;

int main()
{
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();

    gl_init();

    surface_t zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    uint8_t valsbuf[16];
    dma_read(valsbuf, 0x10000000 | ROMHACKED_OFFSET, sizeof(valsbuf));

    uint32_t vals[2];
    memcpy(vals, valsbuf, 8);

    uint32_t pi_addr_offset = 0x10000000 | vals[0];
    uint32_t size = vals[1];

    debugf("pi_addr_offset = 0x%08lX\n", pi_addr_offset);
    debugf("size = 0x%lX\n", size);

    uint8_t *buf = memalign(16, size);
    dma_read(buf, pi_addr_offset, size);
    data_cache_hit_invalidate(buf, size);

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 16; i++)
            debugf("%02hhX ", buf[j * 16 + i]);
        debugf("\n");
    }

    model64_t *model = model64_load_buf(buf, size);

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

        // make the object rotate
        glRotatef(angle, 0, 1, 0);
        angle += 360.0f / 100;

        if (!use_the_dlist || !the_dlist_is_built)
        {
            if (use_the_dlist)
            {
                the_dlist_is_built = true;

                the_dlist = glGenLists(1);
                glNewList(the_dlist, GL_COMPILE);
            }

            model64_draw(model);

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

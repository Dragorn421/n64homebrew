#include <stdlib.h>
#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <usb.h>
#include <math.h>

sprite_t *sprite_n64brew_32x64;

void advanced_quad_with_vertex_arrays(surface_t *disp, bool use_texture)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    GLuint my_texture;
    if (use_texture)
    {
        glGenTextures(1, &my_texture);
        glBindTexture(GL_TEXTURE_2D, my_texture);
        glSpriteTextureN64(GL_TEXTURE_2D, sprite_n64brew_32x64,
                           &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});
    }

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (use_texture)
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, my_texture);
    }

    GLfloat lmodel_ambient[] = {0, 0, 0, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat[4]){1, 0, 0, 1});
    glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){0, 1, 0, 1});
    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[4]){0.5f, 0.5f, 0.5f, 1});
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnableClientState(GL_VERTEX_ARRAY);
    if (use_texture)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    float f = 0.5f;

    struct my_vertex_struct
    {
        GLfloat position[3];
        GLfloat texcoord[2];
        GLfloat normal[3];
        uint32_t color;
    };

    struct my_vertex_struct quad_vertices[] = {
        /* 0 */ {.position = {f, f, 0}, .texcoord = {0.8f, 0.4f}, .normal = {0.f, 0.f, 1.f}, .color = 0xFFFF00FF},
        /* 1 */ {.position = {-f, f, 0}, .texcoord = {0.f, 0.4f}, .normal = {0.f, 0.f, 1.f}, .color = 0x00FFFFFF},
        /* 2 */ {.position = {-f, -f, 0}, .texcoord = {0.f, 1.f}, .normal = {0.f, 0.f, 1.f}, .color = 0xFF00FFFF},
        /* 3 */ {.position = {f, -f, 0}, .texcoord = {0.8f, 1.f}, .normal = {0.f, 0.f, 1.f}, .color = 0xFFFFFFFF},
    };
    GLushort tris_indices[] = {
        0, 1, 2,
        0, 2, 3};

    glVertexPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].position);
    if (use_texture)
    {
        glTexCoordPointer(2, GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].texcoord);
    }
    glNormalPointer(GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].normal);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct my_vertex_struct), &quad_vertices[0].color);

    glDrawElements(GL_TRIANGLES, sizeof(tris_indices) / sizeof(tris_indices[0]), GL_UNSIGNED_SHORT, tris_indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (use_texture)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    gl_context_end();

    rdpq_detach();
}

int main()
{
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    sprite_n64brew_32x64 = sprite_load("rom:/n64brew_32x64.sprite");

    controller_init();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    rdpq_init();

    debug_init(DEBUG_FEATURE_ALL);
    rdpq_debug_start();
    rdpq_debug_log(true);

    bool antialiasing_enabled = false;
    bool use_texture = true;

    while (true)
    {
        surface_t *disp = display_get();

        gl_init();

        if (antialiasing_enabled)
        {
            glEnable(GL_MULTISAMPLE_ARB);
        }
        else
        {
            glDisable(GL_MULTISAMPLE_ARB);
        }

        advanced_quad_with_vertex_arrays(disp, use_texture);

        gl_close();

        rspq_wait();

        char msg[100];

        snprintf(msg, sizeof(msg), "AA: %s (press A)", antialiasing_enabled ? "ON" : "OFF");
        graphics_draw_text(disp, 20, 30, msg);

        snprintf(msg, sizeof(msg), "Tex: %s (press B)", use_texture ? "ON" : "OFF");
        graphics_draw_text(disp, 20, 40, msg);

        display_show(disp);

        bool redraw_demo_piece = false;
        while (!redraw_demo_piece)
        {
            struct controller_data controller_data;

            controller_scan();
            controller_data = get_keys_down();

            if (controller_data.c[0].A)
            {
                antialiasing_enabled = !antialiasing_enabled;
                redraw_demo_piece = true;
            }
            if (controller_data.c[0].B)
            {
                use_texture = !use_texture;
                redraw_demo_piece = true;
            }

            wait_ms(1);
        }
    }
}

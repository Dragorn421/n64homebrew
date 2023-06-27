#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

static surface_t zbuffer;

typedef struct
{
    float position[3];
    float texcoord[2];
    float normal[3];
    uint32_t color;
} vertex_t;

int main()
{
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    rdpq_init();
    gl_init();

    // rdpq_debug_start();
    // rdpq_debug_log(true);

    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    // resized 90x117 -> 32x64
    sprite_t *sprite = sprite_load("rom:/n64brew_32x64.sprite");
    (void)sprite;

    float aspect_ratio = (float)display_get_width() / (float)display_get_height();
    float near_plane = 1.0f;
    float far_plane = 50.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane * aspect_ratio, near_plane * aspect_ratio, -near_plane, near_plane, near_plane, far_plane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    controller_init();

    float angle_degrees=0;

    while (1)
    {
        controller_scan();
        struct controller_data pressed = get_keys_pressed();

        if (pressed.c[0].A)
        {
            debugf("Pressed A %f\n", angle_degrees);
            angle_degrees += 10;
        }

        surface_t *disp = display_get();

        rdpq_attach(disp, &zbuffer);

        gl_context_begin();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            0, 10, 10,
            0, 0, 0,
            0, 1, 0);
        glRotatef(angle_degrees, 0, 1, 0);

        // Set some global render modes that we want to apply to all models
        // glEnable(GL_LIGHTING);
        // glEnable(GL_NORMALIZE);
        // glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);

        // glEnable(GL_TEXTURE_2D);
        // glBindTexture(GL_TEXTURE_2D, textures[texture_index]);

        // render_plane();
        // render_decal();

        rdpq_debug_log_msg("Cube");
        glPushMatrix();
        glTranslatef(0, -1.f, 0);

        // Apply vertex color as material color.
        // Because the cube has colors set per vertex, we can color each face seperately
        // glEnable(GL_COLOR_MATERIAL);

        // Apply to ambient and diffuse material properties
        // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        float cube_size = 5;
        vertex_t cube_vertices[] = {
            {.position = {cube_size, -cube_size, -cube_size}, .texcoord = {0.f, 0.f}, .normal = {1.f, 0.f, 0.f}, .color = 0xFF0000FF},
            {.position = {cube_size, cube_size, -cube_size}, .texcoord = {1.f, 0.f}, .normal = {1.f, 0.f, 0.f}, .color = 0xFF0000FF},
            {.position = {cube_size, cube_size, cube_size}, .texcoord = {1.f, 1.f}, .normal = {1.f, 0.f, 0.f}, .color = 0xFF0000FF},
        };
        uint16_t cube_indices[] = {
            0,
            1,
            2,
            0,
            2,
            3,
        };

        glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), (void *)(0 * sizeof(float) + (void *)cube_vertices));
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), (void *)(3 * sizeof(float) + (void *)cube_vertices));
        glNormalPointer(GL_FLOAT, sizeof(vertex_t), (void *)(5 * sizeof(float) + (void *)cube_vertices));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_t), (void *)(8 * sizeof(float) + (void *)cube_vertices));

        glDrawElements(GL_TRIANGLES, sizeof(cube_indices) / sizeof(uint16_t), GL_UNSIGNED_SHORT, cube_indices);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        // glDisable(GL_COLOR_MATERIAL);

        glPopMatrix();

        // render_skinned(&camera, animation);

        // glBindTexture(GL_TEXTURE_2D, textures[(texture_index + 1)%4]);
        // render_sphere(rotation);

        // glDisable(GL_TEXTURE_2D);
        // glDisable(GL_LIGHTING);
        // render_primitives(rotation);

        gl_context_end();

        rdpq_detach_show();
    }
}
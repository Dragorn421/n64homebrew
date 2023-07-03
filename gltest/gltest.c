#include <stdlib.h>
#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <usb.h>
#include <math.h>

int main()
{
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();

    while (true)
    {
        surface_t *disp = display_get();

        gl_init();
        rdpq_attach(disp, NULL);
        gl_context_begin();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
            glVertex3f(5, 5, 0);
            glVertex3f(-5, 5, 0);
            glVertex3f(5, -5, 0);
        glEnd();

        gl_context_end();
        rdpq_detach();
        rspq_wait();
        gl_close();

        display_show(disp);
    }
}

void one_basic_triangle(surface_t *disp)
{
    // attach the framebuffer (no depth buffer)
    rdpq_attach(disp, NULL);

    // start issuing graphics commands using OpenGL
    gl_context_begin();

    // clear the framebuffer (see http://www.glprogramming.com/red/chapter02.html#name1 )
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw a triangle (see http://www.glprogramming.com/red/chapter02.html#name2 )
    glBegin(GL_TRIANGLES);

    glVertex3f(5, 5, 0);
    glVertex3f(-5, 5, 0);
    glVertex3f(5, -5, 0);

    glEnd();

    // matches gl_context_begin
    gl_context_end();

    // matches rdpq_attach
    rdpq_detach();
}

struct DemoPiece
{
    void (*callback)(surface_t *);
    char *name;
};
#define DEMO_PIECE(function_name)     \
    {                                 \
        function_name, #function_name \
    }
struct DemoPiece demo_pieces[] = {
    DEMO_PIECE(one_basic_triangle),
};
int n_demo_pieces = sizeof(demo_pieces) / sizeof(demo_pieces[0]);

int main_demoinpieces()
{
    debug_init_usblog();

    controller_init();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    rdpq_init();

    int cur_demo_piece_i = 0;

    while (true)
    {
        struct DemoPiece *cur_demo_piece = &demo_pieces[cur_demo_piece_i];

        surface_t *disp = display_get();

        gl_init();

        cur_demo_piece->callback(disp);

        gl_close();

        // wait RSP and RDP done
        rspq_wait();

        graphics_draw_text(disp, 20, 20, cur_demo_piece->name);

        display_show(disp);

        int next_demo_piece_i = cur_demo_piece_i;
        while (next_demo_piece_i == cur_demo_piece_i)
        {
            struct controller_data controller_data;
            controller_read(&controller_data);
            if (controller_data.c[0].up)
            {
                next_demo_piece_i -= 1;
            }
            if (controller_data.c[0].down)
            {
                next_demo_piece_i += 1;
            }
            if (next_demo_piece_i < 0)
            {
                next_demo_piece_i = n_demo_pieces - 1;
            }
            if (next_demo_piece_i >= n_demo_pieces)
            {
                next_demo_piece_i = 0;
            }
            wait_ms(1);
        }
        cur_demo_piece_i = next_demo_piece_i;
    }
}

int main_finalbef()
{
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();
    gl_init();

    surface_t *disp = display_get();

    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, 4.0f / 3.0f, 1.0f, 50.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 10, -20,
        0, 0, 0,
        0, 1, 0);

    dfs_init(DFS_DEFAULT_LOCATION);
    sprite_t *sprite = sprite_load("rom:/n64brew_32x64.sprite");
    sprite_free(sprite);

    glEnable(GL_TEXTURE_2D);

    GLuint mytexture;
    glGenTextures(1, &mytexture);
    glBindTexture(GL_TEXTURE_2D, mytexture);
    glSpriteTextureN64(GL_TEXTURE_2D, sprite, &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(1, 1, 0);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0, 0);
    glVertex3f(5, 5, 0);

    glTexCoord2f(2, 0);
    glVertex3f(-5, 5, 0);

    glTexCoord2f(0, 2);
    glVertex3f(5, -5, 0);

    glEnd();

    gl_context_end();

    rdpq_detach_wait();

    gl_close();

    graphics_draw_text(disp, 20, 20, "hello");

    display_show(disp);

    exit(EXIT_SUCCESS);
}

int main_tf()
{
    debug_init_usblog();
    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();
    gl_init();

    sprite_t *sprite = sprite_load("rom:/n64brew_32x64.sprite");

    surface_t *disp = display_get();

    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, 4.0f / 3.0f, 1.0f, 50.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 10, -20,
        0, 0, 0,
        0, 1, 0);

    GLfloat imagedata[16][16][3];
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            imagedata[y][x][0] = 0.5;
            imagedata[y][x][1] = 0.2;
            imagedata[y][x][2] = 0;
        }
    }

    GLuint mytexture;
    glGenTextures(1, &mytexture);
    glBindTexture(GL_TEXTURE_2D, mytexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_FLOAT, imagedata);

    (void)sprite;
    //    glSpriteTextureN64(GL_TEXTURE_2D, sprite, &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, mytexture);

    // glColor3f(0, 0, 1);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0, 0);
    glVertex3f(5, 5, 0);

    glTexCoord2f(2, 0);
    glVertex3f(-5, 5, 0);

    glTexCoord2f(0, 2);
    glVertex3f(5, -5, 0);

    glEnd();

    gl_context_end();

    rdpq_detach_show();

    exit(EXIT_SUCCESS);
}

int main_idk()
{
    debug_init_usblog();
    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();
    gl_init();

    sprite_t *sprite = sprite_load("rom:/n64brew_32x64.sprite");

    surface_t *disp = display_get();

    rdpq_attach(disp, NULL);

    gl_context_begin();
    glEnable(GL_MULTISAMPLE_ARB); // antialiasing

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, 4.0f / 3.0f, 1.0f, 50.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 10, -20,
        0, 0, 0,
        0, 1, 0);

    // GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat lmodel_ambient[] = {0, 0, 0, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    // glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat[4]){1, 1, 1, 1});
    glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){0, 0, 0, 1});
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){1, 1, 1, 1});
    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[4]){6, 10, 0, 1});
    // glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[4]){20, 3, 0, 1});
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
    // glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
    // glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 4);
    // glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, -0.2);
    glEnable(GL_LIGHT0);

    // glColor3ub(255, 255, 0);

    // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat[4]){1, 1, 0, 1});
    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat[4]){0, 0.5, 0, 1});

    // glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // glEnable(GL_COLOR_MATERIAL);
    // glColorMaterial(GL_FRONT_AND_BACK, );

    GLuint mytexture;
    debugf("glIsTexture(0xa00bf1a0) = %d\n", glIsTexture(0xa00bf1a0));
    glGenTextures(1, &mytexture);
    debugf("mytexture = %08lx\n", mytexture);
    debugf("glIsTexture(mytexture) = %d\n", glIsTexture(mytexture));
    glBindTexture(GL_TEXTURE_2D, mytexture);
    debugf("glIsTexture(mytexture) = %d\n", glIsTexture(mytexture));
    glSpriteTextureN64(GL_TEXTURE_2D, sprite, &(rdpq_texparms_t){0});

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glEnable(GL_TEXTURE_2D);

    debugf("glIsList(1) = %d\n", glIsList(1));

    debugf("tri_dlist = glGenLists(1)\n");
    GLuint tri_dlist = glGenLists(1);

    debugf("tri_dlist = %ld\n", tri_dlist);
    debugf("glIsList(tri_dlist) = %d\n", glIsList(tri_dlist));

    debugf("glCallList(tri_dlist)\n");
    glCallList(tri_dlist); // no-op according to spec

    debugf("glIsList(tri_dlist) = %d\n", glIsList(tri_dlist));

    debugf("glNewList(tri_dlist, GL_COMPILE)\n");
    glNewList(tri_dlist, GL_COMPILE);

    debugf("glIsList(tri_dlist) = %d\n", glIsList(tri_dlist));

    debugf("glBegin(GL_TRIANGLES)\n");
    glBegin(GL_TRIANGLES);
    glNormal3f(1, 1, 0);
    glTexCoord2f(0, 0);
    glVertex3f(5, 5, 0);

    glColor3f(0, 1, 0);
    glNormal3f(-1, 1, 0);
    glTexCoord2f(2, 0);
    glVertex3f(-5, 5, 0);

    glColor3f(0, 0, 1);
    glNormal3f(1, -1, 0);
    glTexCoord2f(0, 2);
    glVertex3f(5, -5, 0);
    glEnd();

    debugf("glEndList\n");
    glEndList();

    debugf("glIsList(tri_dlist) = %d\n", glIsList(tri_dlist));

    glColor3f(1, 0, 0);
    glCallList(tri_dlist);
    glTranslatef(0, -5, -10);
    glColor3f(1, 1, 1);
    glCallList(tri_dlist);

    gl_context_end();

    rdpq_detach_show();

    exit(EXIT_SUCCESS);
}

int main_____________________________()
{
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();
    gl_init();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, 4.0f / 3.0f, 1.0f, 50.0f);

    surface_t *disp = display_get();

    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 10, -20,
        0, 0, 0,
        0, 1, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glEnableClientState(GL_COLOR_ARRAY);
    // glColor3b(255, 0, 0);

    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    // GLfloat light_position[] = {2.0, 4.0, 1.0, 0.0}; // directional (w = 0)
    GLfloat light_position[] = {15.0, 2.0, 1.0, 1.0}; // positional (w != 0)

    glColor3f(1, 1, 1);
    glColor3ub(255, 255, 255);
    glBegin(GL_POINTS);
    glVertex3f(light_position[0], light_position[1], light_position[2]);
    glEnd();

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat[4]){0.2, 0.2, 0.2, 1});
    glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){0, 1, 0, 1});

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float quad_side_half_length = 5;
#define INVSQRT2 0.707f
    struct
    {
        GLfloat position[3];
        GLfloat normal[3];
        GLbyte color[4];
    } quad_vertices[] = {
        /*

           0
          /|     y^
         / 3      | ^x
        1 /       |/
        |/        .-> z
        2

        */
        /* 0 */ {.position = {quad_side_half_length, quad_side_half_length, 0}, .normal = {INVSQRT2, INVSQRT2, 0}, .color = {255, 0, 0, 255}},
        /* 1 */ {.position = {-quad_side_half_length, quad_side_half_length, 0}, .normal = {-INVSQRT2, INVSQRT2, 0}, .color = {0, 255, 0, 255}},
        /* 2 */ {.position = {-quad_side_half_length, -quad_side_half_length, 0}, .normal = {-INVSQRT2, -INVSQRT2, 0}, .color = {0, 0, 255, 255}},
        /* 3 */ {.position = {quad_side_half_length, -quad_side_half_length, 0}, .normal = {INVSQRT2, -INVSQRT2, 0}, .color = {255, 0, 255, 255}},
    };
    GLushort cube_indices[] = {
        0,
        1,
        2,
        0,
        2,
        3,
    };

    glVertexPointer(3, GL_FLOAT, sizeof(quad_vertices[0]), &quad_vertices[0].position);
    glNormalPointer(GL_FLOAT, sizeof(quad_vertices[0]), &quad_vertices[0].normal);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(quad_vertices[0]), &quad_vertices[0].color);

    glDrawElements(GL_TRIANGLES, sizeof(cube_indices) / sizeof(uint16_t), GL_UNSIGNED_SHORT, cube_indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    gl_context_end();

    rdpq_detach_show();

    exit(EXIT_SUCCESS);
}

int main____()
{
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    rdpq_init();
    gl_init();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, 4.0f / 3.0f, 1.0f, 50.0f);

    surface_t *disp = display_get();

    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0, 10, 10,
        0, 0, 0,
        0, 1, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    // glColor3b(255, 0, 0);

    // GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 3.0, 1.0, 0.0};

    // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat[4]){0.2, 0.2, 0.2, 1});
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){0, 1, 0, 1});

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // glDisable(GL_LIGHTING);

    float quad_side_half_length = 5;
#define INVSQRT2 0.707f
    struct
    {
        GLfloat position[3];
        GLfloat normal[3];
        GLbyte color[4];
    } quad_vertices[] = {
        /*

           0
          /|     y^
         / 3      | ^x
        1 /       |/
        |/        .-> z
        2

        */
        /* 0 */ {.position = {quad_side_half_length, quad_side_half_length, 0}, .normal = {INVSQRT2, INVSQRT2, 0}, .color = {255, 0, 0, 255}},
        /* 1 */ {.position = {-quad_side_half_length, quad_side_half_length, 0}, .normal = {-INVSQRT2, INVSQRT2, 0}, .color = {0, 255, 0, 255}},
        /* 2 */ {.position = {-quad_side_half_length, -quad_side_half_length, 0}, .normal = {-INVSQRT2, -INVSQRT2, 0}, .color = {0, 0, 255, 255}},
        /* 3 */ {.position = {quad_side_half_length, -quad_side_half_length, 0}, .normal = {INVSQRT2, -INVSQRT2, 0}, .color = {255, 0, 255, 255}},
    };
    GLushort cube_indices[] = {
        0,
        1,
        2,
        0,
        2,
        3,
    };

    glVertexPointer(3, GL_FLOAT, sizeof(quad_vertices[0]), &quad_vertices[0].position);
    glNormalPointer(GL_FLOAT, sizeof(quad_vertices[0]), &quad_vertices[0].normal);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(quad_vertices[0]), &quad_vertices[0].color);

    glDrawElements(GL_TRIANGLES, sizeof(cube_indices) / sizeof(uint16_t), GL_UNSIGNED_SHORT, cube_indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    gl_context_end();

    rdpq_detach_show();

    exit(EXIT_SUCCESS);
}

static surface_t zbuffer;

typedef struct
{
    float position[3];
    float texcoord[2];
    float normal[3];
    uint32_t color;
} vertex_t;

int main_()
{
    debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    rdpq_init();
    gl_init();

    int tmem_size = 4 * 1024;
    surface_t tex_zeros = surface_alloc(FMT_I8, 64, tmem_size / 64);
    memset(tex_zeros.buffer, 0, tmem_size);
    rdpq_tex_upload(TILE0, &tex_zeros, NULL);

    // rdpq_debug_start();
    // rdpq_debug_log(true);

    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    // resized 90x117 -> 32x64
    sprite_t *sprite = sprite_load("rom:/n64brew_32x64.sprite");
    (void)sprite;

    float aspect_ratio = 4.0f / 3.0f; //(float)display_get_width() / (float)display_get_height();
    float near_plane_z = 1.0f;
    float far_plane_z = 50.0f;
    float view_angle = 80 * (M_PI / 180);
    float near_plane_x = near_plane_z * tanf(view_angle / 2); // view_angle = 2 * atan(near_plane_x / near_plane_z)
    float near_plane_y = near_plane_x / aspect_ratio;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // GLfloat mat_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

    GLuint textures[4];
    glGenTextures(4, textures);
    for (int i = 0; i < 4; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSpriteTextureN64(GL_TEXTURE_2D, sprite, &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});
    }

    controller_init();

    float angle_degrees = 0;

    while (1)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (get_ticks_ms() % 1000 < 500)
        {
            glFrustum(-near_plane_x, near_plane_x, -near_plane_y, near_plane_y, near_plane_z, far_plane_z);
        }
        else
        {
            // view_angle = 2 * atan(near_plane_x / near_plane_z)
            // fovy = 2 * atan(near_plane_y / near_plane_z)
            gluPerspective(2 * atanf(near_plane_y / near_plane_z) * (180 / M_PI), aspect_ratio, near_plane_z, far_plane_z);
        }

        controller_scan();
        struct controller_data pressed = get_keys_pressed();

        if (pressed.c[0].A)
        {
            debugf("Pressed A %f\n", angle_degrees);
            angle_degrees += 10;
        }
        uint32_t header = usb_poll();
        if (header != 0)
        {
            int type = USBHEADER_GETTYPE(header);
            int size = USBHEADER_GETSIZE(header);
            uint8_t data[size + 1]; // note: bad idea if size is "a lot"
            usb_read(data, size);
            data[size] = 0;
            debugf("usb_poll() -> %08lX (type=%d, size=%d)\n", header, type, size);
            switch (type)
            {
            case DATATYPE_TEXT:
                debugf("    text\n");
                debugf("    data = \"%s\"\n", data);
                break;
            default:
                debugf("    not text\n");
                debugf("    data =");
                for (int i = 0; i < size; i++)
                    debugf(" %02X", data[i]);
                debugf("\n");
                break;
            }
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

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        // glEnable(GL_TEXTURE_2D);

        // render_plane();
        // render_decal();

        rdpq_debug_log_msg("Cube");
        glPushMatrix();
        glTranslatef(0, 0, 0);

        // Apply vertex color as material color.
        // Because the cube has colors set per vertex, we can color each face seperately
        // glEnable(GL_COLOR_MATERIAL);

        // Apply to ambient and diffuse material properties
        // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        float quad_side_half_length = 5;
        vertex_t quad_vertices[] = {
            /*

               0
              /|     y^
             / 3      | ^x
            1 /       |/
            |/        .-> z
            2

            */
            /* 0 */ {.position = {quad_side_half_length, quad_side_half_length, 0}, .texcoord = {1.f, 0.f}, .normal = {0.f, 0.f, 1.f}, .color = 0xFF0000FF},
            /* 1 */ {.position = {-quad_side_half_length, quad_side_half_length, 0}, .texcoord = {0.f, 0.f}, .normal = {0.f, 0.f, 1.f}, .color = 0x00FF00FF},
            /* 2 */ {.position = {-quad_side_half_length, -quad_side_half_length, 0}, .texcoord = {0.f, 1.f}, .normal = {0.f, 0.f, 1.f}, .color = 0x0000FFFF},
            /* 3 */ {.position = {quad_side_half_length, -quad_side_half_length, 0}, .texcoord = {1.f, 1.f}, .normal = {0.f, 0.f, 1.f}, .color = 0xFF00FFFF},
        };
        uint16_t cube_indices[] = {
            0,
            1,
            2,
            0,
            2,
            3,
        };

        glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), (void *)(0 * sizeof(float) + (void *)quad_vertices));
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), (void *)(3 * sizeof(float) + (void *)quad_vertices));
        glNormalPointer(GL_FLOAT, sizeof(vertex_t), (void *)(5 * sizeof(float) + (void *)quad_vertices));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_t), (void *)(8 * sizeof(float) + (void *)quad_vertices));

        glDrawElements(GL_TRIANGLES, sizeof(cube_indices) / sizeof(uint16_t), GL_UNSIGNED_SHORT, cube_indices);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

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
#include <stdlib.h>
#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <usb.h>
#include <math.h>

sprite_t *sprite_n64brew_32x64;
surface_t zbuffer;

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

    float f = 0.5f;
    glVertex3f(f, f, 0);
    glVertex3f(-f, f, 0);
    glVertex3f(f, -f, 0);

    // matches glBegin
    glEnd();

    // matches gl_context_begin
    gl_context_end();

    // matches rdpq_attach
    rdpq_detach();
}

void one_colored_triangle(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // change the color for the vertices after (see http://www.glprogramming.com/red/chapter02.html#name2 )
    // (the default color is white)
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    float f = 0.5f;

    glVertex3f(f, f, 0);
    glVertex3f(-f, f, 0);

    // can also be used in-between a glBegin/glEnd pair
    glColor3f(0.0f, 0.0f, 1.0f);

    glVertex3f(f, -f, 0);

    glEnd();

    gl_context_end();

    rdpq_detach();
}

void one_textured_triangle(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    // "Define" the texture for OpenGL
    // (see http://www.glprogramming.com/red/chapter09.html#name4 )
    // Typically you would do this once and reuse my_texture every frame,
    // but this is a one-function demo
    GLuint my_texture;
    // Get a "name" (an identifier) for the texture object
    glGenTextures(1, &my_texture);
    // Make the texture object identified by the my_texture name, active
    glBindTexture(GL_TEXTURE_2D, my_texture);
    // libdragon-specific texture loading function: load a sprite into the active texture object
    // The regular OpenGL texture loading functions are available too but harder to use.
    glSpriteTextureN64(GL_TEXTURE_2D, sprite_n64brew_32x64,
                       &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glClearColor(0.1f, 0.2f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable texture mapping (see http://www.glprogramming.com/red/chapter09.html#name1 )
    glEnable(GL_TEXTURE_2D);
    // Set how the texture applies to the drawn geometry
    // (see http://www.glprogramming.com/red/chapter09.html#name5 )
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // Make the texture object active, of the texture to use
    // Note: technically this glBindTexture call isn't needed in this function,
    //       because the texture object tied to my_texture is still active from
    //       the above identical line. However in a real application you would
    //       define the texture once, store the texture name (the my_texture value),
    //       and activate it right before drawing.
    glBindTexture(GL_TEXTURE_2D, my_texture);

    glBegin(GL_TRIANGLES);

    float f = 0.5f;

    // Set texture coordinates (also known as UVs)
    // (see http://www.glprogramming.com/red/chapter09.html#name6 )
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(f, f, 0);
    glTexCoord2f(-2.0f, 0.0f);
    glVertex3f(-f, f, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(f, -f, 0);

    glEnd();

    gl_context_end();

    rdpq_detach();
}

void one_colored_textured_triangle(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    GLuint my_texture;
    glGenTextures(1, &my_texture);
    glBindTexture(GL_TEXTURE_2D, my_texture);
    glSpriteTextureN64(GL_TEXTURE_2D, sprite_n64brew_32x64,
                       &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glClearColor(0.1f, 0.2f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    // Use GL_MODULATE instead of GL_REPLACE
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, my_texture);

    glBegin(GL_TRIANGLES);

    float f = 0.5f;

    // Setting colors as well as texture coordinates
    // Also using glColor3ub "unsigned byte" instead of glColor3f "float" to show different argument types.
    glColor3ub(255, 255, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(f, f, 0);

    glTexCoord2f(-2.0f, 0.0f);
    glVertex3f(-f, f, 0);

    glColor3ub(255, 0, 255);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(f, -f, 0);

    glEnd();

    gl_context_end();

    rdpq_detach();
}

void one_textured_cutout_triangle(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    GLuint my_texture;
    glGenTextures(1, &my_texture);
    glBindTexture(GL_TEXTURE_2D, my_texture);
    glSpriteTextureN64(GL_TEXTURE_2D, sprite_n64brew_32x64,
                       &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glClearColor(0.1f, 0.2f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, my_texture);

    // Only draw pixels with alpha > 0.5
    // (see http://www.glprogramming.com/red/chapter10.html#name2 )
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);

    glBegin(GL_TRIANGLES);

    float f = 0.5f;

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(f, f, 0);

    glTexCoord2f(-2.0f, 0.0f);
    glVertex3f(-f, f, 0);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(f, -f, 0);

    glEnd();

    gl_context_end();

    rdpq_detach();
}

void one_translucent_textured_triangle(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    GLuint my_texture;
    glGenTextures(1, &my_texture);
    glBindTexture(GL_TEXTURE_2D, my_texture);
    glSpriteTextureN64(GL_TEXTURE_2D, sprite_n64brew_32x64,
                       &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glClearColor(0.1f, 0.2f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, my_texture);

    // Blend the drawn colors with the framebuffer using the indicated factors
    // (see http://www.glprogramming.com/red/chapter06.html#name1 )
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLES);

    float f = 0.5f;

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(f, f, 0);

    glTexCoord2f(-2.0f, 0.0f);
    glVertex3f(-f, f, 0);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(f, -f, 0);

    glEnd();

    gl_context_end();

    rdpq_detach();
}

void triangles_with_vertex_arrays(surface_t *disp)
{
    float x, y, f = 0.2f;

    rdpq_attach(disp, NULL);

    gl_context_begin();

    GLuint my_texture;
    glGenTextures(1, &my_texture);
    glBindTexture(GL_TEXTURE_2D, my_texture);
    glSpriteTextureN64(GL_TEXTURE_2D, sprite_n64brew_32x64,
                       &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    x = -0.8f;
    y = 0.4f;
    // Similar to one_basic_triangle but with vertex arrays
    {
        // (see http://www.glprogramming.com/red/chapter02.html#name6 )

        // Define a structure for the vertex data
        // Note: Typically such a structure would be defined for a larger scope
        //       and reused, but it's defined in the function's scope for the purpose
        //       of this demo.
        struct my_vertex_struct
        {
            GLfloat position[3];
        };
        // Define the data for vertices
        struct my_vertex_struct verts[] = {
            /* 0 */ {{x + f, y + f, 0}},
            /* 1 */ {{x - f, y + f, 0}},
            /* 2 */ {{x + f, y - f, 0}},
        };
        // Define indices refering to the vertices data
        GLushort indices[] = {0, 1, 2};

        // Enable the vertex coordinates array
        glEnableClientState(GL_VERTEX_ARRAY);
        // Set the vertex coordinates array
        // "3, GL_FLOAT" because my_vertex_struct.position is a GLfloat[3]
        glVertexPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &verts[0].position);
        // "3, GL_UNSIGNED_SHORT" because indices is a GLushort[3]
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices);
        // Disable the vertex coordinates array
        // While this is not strictly required, this (and in general disabling locally-enabled
        // states) is a good idea to leave a known base state for later commands.
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    x = -0.4f;
    y = 0.4f;
    // Similar to one_colored_triangle but with vertex arrays
    {
        // The vertex structure now has color
        // Note the order of the struct members can be shuffled around
        struct my_vertex_struct
        {
            GLfloat position[3];
            GLfloat color[3];
        };
        struct my_vertex_struct verts[] = {
            /* 0 */ {{x + f, y + f, 0}, {1.0f, 0.0f, 0.0f}},
            /* 1 */ {{x - f, y + f, 0}, {1.0f, 0.0f, 0.0f}},
            /* 2 */ {{x + f, y - f, 0}, {0.0f, 0.0f, 1.0f}},
        };
        GLushort indices[] = {0, 1, 2};

        glEnableClientState(GL_VERTEX_ARRAY);
        // Enable the color array
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &verts[0].position);
        // Set the color array
        // "3, GL_FLOAT" because my_vertex_struct.color is a GLfloat[3]
        glColorPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &verts[0].color);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices);
        glDisableClientState(GL_VERTEX_ARRAY);
        // Disable the color array
        glDisableClientState(GL_COLOR_ARRAY);
    }

    // Skipping over one_textured_triangle

    x = 0.0f;
    y = 0.4f;
    // Similar to one_colored_textured_triangle but with vertex arrays
    {
        // The vertex structure now has texture coordinates
        // Note the order of the struct members can be shuffled around
        struct my_vertex_struct
        {
            GLfloat position[3];
            GLubyte color[3]; // now GLubyte
            GLfloat texcoords[2];
        };
        struct my_vertex_struct verts[] = {
            /* 0 */ {{x + f, y + f, 0}, {255, 255, 0}, {0.0f, 0.0f}},
            /* 1 */ {{x - f, y + f, 0}, {255, 255, 0}, {-2.0f, 0.0f}},
            /* 2 */ {{x + f, y - f, 0}, {255, 0, 255}, {0.0f, 1.0f}},
        };
        GLushort indices[] = {0, 1, 2};

        // Activate the texture object and enable texturing
        // This can be done at any point before glDrawElements
        // (it could be done right after the glTexCoordPointer call for example)
        glBindTexture(GL_TEXTURE_2D, my_texture);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        // Enable the texture coordinates array
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &verts[0].position);
        // now GL_UNSIGNED_BYTE
        glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(struct my_vertex_struct), &verts[0].color);
        // Set the texture coordinates array
        // "2, GL_FLOAT" because my_vertex_struct.texcoords is a GLfloat[2]
        glTexCoordPointer(2, GL_FLOAT, sizeof(struct my_vertex_struct), &verts[0].texcoords);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        // Disable the texture coordinates array
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glDisable(GL_TEXTURE_2D);
    }

    gl_context_end();

    rdpq_detach();
}

void colored_quad_with_vertex_arrays(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw (two) triangles forming a quad, using vertex arrays

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    float f = 0.5f;

    struct my_vertex_struct
    {
        GLfloat position[3];
        uint32_t color;
    };

    struct my_vertex_struct quad_vertices[] = {
        /*
        Diagram of the position of the four vertices:

           0
          /|     y^
         / 3      | ^x
        1 /       |/
        |/        .--> z
        2

        With the default projection and modelview matrices,
        when looking at the image on a screen:
        x = right
        y = up
        z = towards the human
        */
        /* 0 */ {.position = {f, f, 0}, .color = 0xFFFF00FF},
        /* 1 */ {.position = {-f, f, 0}, .color = 0x00FFFFFF},
        /* 2 */ {.position = {-f, -f, 0}, .color = 0xFF00FFFF},
        /* 3 */ {.position = {f, -f, 0}, .color = 0xFFFFFFFF},
    };
    GLushort tris_indices[] = {
        // Note both these triangles are carefully chosen so the indices wrap around +z,
        // making the front side of both triangles towards +z (towards the human).
        // This would matter if we were to use backface culling.
        // Vertex indices of first triangle
        0, 1, 2,
        // Vertex indices of second triangle
        0, 2, 3};

    glVertexPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].position);
    // "4, GL_UNSIGNED_BYTE" because my_vertex_struct.color is a uint32_t and we cheat type-punning it as uint8_t[4] thanks to big-endianness
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct my_vertex_struct), &quad_vertices[0].color);

    glDrawElements(GL_TRIANGLES, sizeof(tris_indices) / sizeof(tris_indices[0]), GL_UNSIGNED_SHORT, tris_indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    gl_context_end();

    rdpq_detach();
}

void quad_transform_rotated(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // (see http://www.glprogramming.com/red/chapter03.html#name1 )
    // Make the following transformations affect the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    // Set the matrix to the identity
    glLoadIdentity();
    // Multiply the transformation by a rotation of 45 degrees around (0,0,1) == +z
    // Since +z is towards the human, this means a counter-clockwise rotation.
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    float f = 0.5f;

    struct my_vertex_struct
    {
        GLfloat position[3];
        uint32_t color;
    };

    struct my_vertex_struct quad_vertices[] = {
        /* 0 */ {.position = {f, f, 0}, .color = 0xFFFF00FF},
        /* 1 */ {.position = {-f, f, 0}, .color = 0x00FFFFFF},
        /* 2 */ {.position = {-f, -f, 0}, .color = 0xFF00FFFF},
        /* 3 */ {.position = {f, -f, 0}, .color = 0xFFFFFFFF},
    };
    GLushort tris_indices[] = {
        0, 1, 2,
        0, 2, 3};

    glVertexPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].position);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct my_vertex_struct), &quad_vertices[0].color);

    glDrawElements(GL_TRIANGLES, sizeof(tris_indices) / sizeof(tris_indices[0]), GL_UNSIGNED_SHORT, tris_indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    gl_context_end();

    rdpq_detach();
}

void quad_display_list(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // (see http://www.glprogramming.com/red/chapter07.html )
    // Allocate one display list index
    GLuint my_quad_dlist = glGenLists(1);
    // Start recording into the display list.
    // The commands will only be recorded, not executed (nothing drawn).
    glNewList(my_quad_dlist, GL_COMPILE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    float f = 0.5f;

    struct my_vertex_struct
    {
        GLfloat position[3];
        uint32_t color;
    };

    struct my_vertex_struct quad_vertices[] = {
        /* 0 */ {.position = {f, f, 0}, .color = 0xFFFF00FF},
        /* 1 */ {.position = {-f, f, 0}, .color = 0x00FFFFFF},
        /* 2 */ {.position = {-f, -f, 0}, .color = 0xFF00FFFF},
        /* 3 */ {.position = {f, -f, 0}, .color = 0xFFFFFFFF},
    };
    GLushort tris_indices[] = {
        0, 1, 2,
        0, 2, 3};

    glVertexPointer(3, GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].position);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct my_vertex_struct), &quad_vertices[0].color);

    glDrawElements(GL_TRIANGLES, sizeof(tris_indices) / sizeof(tris_indices[0]), GL_UNSIGNED_SHORT, tris_indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    // Stop recording the display list
    // matches glNewList
    glEndList();

    // The my_quad_dlist display list can now be called to draw the quad
    // To showcase this the dlist will be drawn several times with different transformations

    // Note:
    // The quads (which are supposed to be squares) will appear sheared due to the
    // default projection not taking into account the aspect ratio and stretching the
    // screen horizontally.
    // Enable the following code to make the projection similar to default but
    // with the correct aspect ratio.
    if (0)
    {
        float aspect_ratio = (float)display_get_width() / (float)display_get_height();
        double h = 1.0;
        double w = h * aspect_ratio;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-w, w, -h, h, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslatef(0.0f, 0.4f, 0.0f);
    glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.7f, 0.4f, 1.0f);
    glCallList(my_quad_dlist);

    glLoadIdentity();
    glTranslatef(-0.5f, -0.3f, 0.0f);
    glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.8f, 1.0f);
    glCallList(my_quad_dlist);

    glLoadIdentity();
    glTranslatef(0.5f, -0.6f, 0.0f);
    glRotatef(42.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.2f, 0.3f, 1.0f);
    glCallList(my_quad_dlist);

    glLoadIdentity();
    glTranslatef(0.5f, -0.1f, 0.0f);
    glScalef(0.4f, 0.4f, 1.0f);
    glCallList(my_quad_dlist);

    gl_context_end();

    rdpq_detach();
}

void box_perspective(surface_t *disp)
{
    float f = 0.5f;

    // Also attach a zbuffer, to prevent drawing far faces over near faces
    rdpq_attach(disp, &zbuffer);

    gl_context_begin();

    // Enable using the zbuffer
    // (see http://www.glprogramming.com/red/chapter05.html#name1 )
    glEnable(GL_DEPTH_TEST);

    glClearColor(0, 0, 0, 1);
    // Also clear the zbuffer in addition to the framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint my_quad_dlist = glGenLists(1);
    // Record my_quad_dlist
    {
        glNewList(my_quad_dlist, GL_COMPILE);

        glEnableClientState(GL_VERTEX_ARRAY);

        GLfloat quad_vertices[][3] = {
            /* 0 */ {f, f, 0},
            /* 1 */ {-f, f, 0},
            /* 2 */ {-f, -f, 0},
            /* 3 */ {f, -f, 0},
        };
        GLushort tris_indices[] = {
            0, 1, 2,
            0, 2, 3};

        glVertexPointer(3, GL_FLOAT, sizeof(quad_vertices[0]), &quad_vertices[0]);

        glDrawElements(GL_TRIANGLES, sizeof(tris_indices) / sizeof(tris_indices[0]), GL_UNSIGNED_SHORT, tris_indices);

        glDisableClientState(GL_VERTEX_ARRAY);

        glEndList();
    }

    // Set the projection matrix to a perspective projection
    // (see http://www.glprogramming.com/red/chapter03.html#name3 )
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, (float)display_get_width() / (float)display_get_height(), 0.1f, 10.0f);

    // Draw a box (cube) by drawing the my_quad_dlist square repeatedly with different transforms
    // for each side of the cube.
    // gluLookAt is used to position the "camera" at the start,
    // then the transforms for individual faces are wrapped in glPushMatrix/glPopMatrix
    // which allows going back to the initial camera matrix each time.
    // (see http://www.glprogramming.com/red/chapter03.html#name2 )
    // Note: this isn't a practical way to draw a model and in a real application you would just
    //       have the whole cube model as arrays of vertices and indices, but doing it this way
    //       is more interesting for the purpose of a demo compared to having big arrays.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0f, 2.0f, 3.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    // +x
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glCallList(my_quad_dlist);
    glPopMatrix();

    // -x
    glColor3f(0.5f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glCallList(my_quad_dlist);
    glPopMatrix();

    // +y
    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glCallList(my_quad_dlist);
    glPopMatrix();

    // -y
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, -f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glCallList(my_quad_dlist);
    glPopMatrix();

    // +z
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, f);
    glCallList(my_quad_dlist);
    glPopMatrix();

    // -z
    glColor3f(0.0f, 0.0f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glCallList(my_quad_dlist);
    glPopMatrix();

    gl_context_end();

    rdpq_detach();
}

void box_translucent(surface_t *disp)
{
    float f = 0.5f;

    rdpq_attach(disp, NULL);

    gl_context_begin();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glCullFace(GL_BACK);

    GLuint my_cube_dlist = glGenLists(1);
    {
        glNewList(my_cube_dlist, GL_COMPILE);

        struct my_vertex_struct
        {
            GLfloat pos[3];
            GLubyte color[4];
        };

        struct my_vertex_struct vertices[6 * 4];
        GLushort quads_indices[6 * 4];

        // Generate a cube model. The cube edges are 2*f long.
        // (easier for me than writing the model by hand, and show off)
        // for each quad face of the cube
        for (int i = 0; i < 6; i++)
        {
            GLfloat axis[3] = {0};
            GLfloat pos[3];
            GLubyte color[4] = {0};

            // The outwards normal of the face
            axis[i / 2] = i % 2 == 0 ? 1.0f : -1.0f;

            pos[0] = axis[0] < 0.0f ? -f : f;
            pos[1] = axis[1] < 0.0f ? -f : f;
            pos[2] = axis[2] < 0.0f ? -f : f;

            color[i / 2] = i % 2 == 0 ? 255 : 127;
            color[3] = 50;

            // for each vertex in the face
            for (int j = 0; j < 4; j++)
            {
                int n = i * 4 + j;
                memcpy(&vertices[n].pos, pos, sizeof(pos));
                quads_indices[n] = n;

                memcpy(&vertices[n].color, color, sizeof(color));

                // next_pos = axis cross pos + f*axis
                GLfloat next_pos[3] = {
                    axis[1] * pos[2] - axis[2] * pos[1] + f * axis[0],
                    axis[2] * pos[0] - axis[0] * pos[2] + f * axis[1],
                    axis[0] * pos[1] - axis[1] * pos[0] + f * axis[2],
                };
                memcpy(&pos, next_pos, sizeof(next_pos));
            }
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, sizeof(vertices[0]), &vertices[0].pos);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertices[0]), &vertices[0].color);

        glDrawElements(GL_QUADS, sizeof(quads_indices) / sizeof(quads_indices[0]), GL_UNSIGNED_SHORT, quads_indices);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        glEndList();
    }

    // Draw a white rectangle as a background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1, 1, 1);
    glRectf(-0.8f, -0.5f, 0.8f, 0.5f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, (float)display_get_width() / (float)display_get_height(), 0.1f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0f, 1.5f, 2.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCallList(my_cube_dlist);

    gl_context_end();

    rdpq_detach();
}

/*
this demo piece is a bit involved
You may want to first comment out glEnable(GL_LIGHTING) in order to see
what happens without lighting:
A textured and colored quad is shown with the corners being colored in:
(mimicking the layout on screen)
    cyan     yellow
    magenta   white
With lighting (
    ambient red,
    and diffuse green from (0.5,0.5,0.5) which is in front of the top-right corner
):
    green    yellow
    red      orange
- Since there is no blue light, the blue disappeared from the colors:
    cyan -> green
    magenta -> red
    white -> yellow
- Since there is an ambient red light
(and the attenuation factors are left at default values which is no attenuation),
all vertices are lit in ambient red.
- The diffuse green light hits directly the top-right corner, lighting it fully in green.
This results in the top-right corner staying yellow
The top-left and bottom-right corners are partially lit in green, which results in
    dark green
    and orange (full red and a bit of green)
 */
void advanced_quad_with_vertex_arrays(surface_t *disp)
{
    rdpq_attach(disp, NULL);

    gl_context_begin();

    GLuint my_texture;
    glGenTextures(1, &my_texture);
    glBindTexture(GL_TEXTURE_2D, my_texture);
    glSpriteTextureN64(GL_TEXTURE_2D, sprite_n64brew_32x64,
                       &(rdpq_texparms_t){.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE});

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, my_texture);

    // "Disable" global ambient light (set color to black)
    // (see www.glprogramming.com/red/chapter05.html#name5 )
    GLfloat lmodel_ambient[] = {0, 0, 0, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    // Enable lighting
    // (see www.glprogramming.com/red/chapter05.html#name5 )
    glEnable(GL_LIGHTING);

    // Configure the first (LIGHT0) light source
    // (see www.glprogramming.com/red/chapter05.html#name5 )
    // Ambient red (light vertices in red regardless of their normals and the direction they're in)
    glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat[4]){1, 0, 0, 1});
    // Diffuse green (light vertices in green, depending on the direction to the light lining up with the normals)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat[4]){0, 1, 0, 1});
    // Be a positional light source (w != 0) from the specified coordinates
    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[4]){0.5f, 0.5f, 0.5f, 1});
    // Enable LIGHT0
    glEnable(GL_LIGHT0);

    // Use the colors for the ambient and diffuse colors
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
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
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].texcoord);
    glNormalPointer(GL_FLOAT, sizeof(struct my_vertex_struct), &quad_vertices[0].normal);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct my_vertex_struct), &quad_vertices[0].color);

    glDrawElements(GL_TRIANGLES, sizeof(tris_indices) / sizeof(tris_indices[0]), GL_UNSIGNED_SHORT, tris_indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    gl_context_end();

    rdpq_detach();
}

// store a callback and a name to display (the function's name) for each "demo piece"
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
    DEMO_PIECE(one_colored_triangle),
    DEMO_PIECE(one_textured_triangle),
    DEMO_PIECE(one_colored_textured_triangle),
    DEMO_PIECE(one_textured_cutout_triangle),
    DEMO_PIECE(one_translucent_textured_triangle),
    DEMO_PIECE(triangles_with_vertex_arrays),
    DEMO_PIECE(colored_quad_with_vertex_arrays),
    DEMO_PIECE(quad_transform_rotated),
    DEMO_PIECE(quad_display_list),
    DEMO_PIECE(box_perspective),
    DEMO_PIECE(box_translucent),
    // TODO simpler lighting examples
    DEMO_PIECE(advanced_quad_with_vertex_arrays),
};
int n_demo_pieces = sizeof(demo_pieces) / sizeof(demo_pieces[0]);

int main()
{
    debug_init_usblog();

    // Initialize the rom fileystem, which lets us access files in the rom using rom:/ paths.
    // In this demo the filesystem is generated in build/file_system.dfs from build/filesystem/
    // See "rom filesystem" in the demo's Makefile for details on the build process.
    dfs_init(DFS_DEFAULT_LOCATION);

    // Load a sprite from the rom filesystem, to be used as a texture in demo pieces.
    sprite_n64brew_32x64 = sprite_load("rom:/n64brew_32x64.sprite");

    controller_init();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    // Allocate a zbuffer for use by the demo pieces if needed.
    // Z-buffer values are two bytes, use FMT_RGBA16 (a two bytes per pixel image format) as a hack.
    // Note: placed after the display_init call so display_get_width/height return the expected values.
    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    rdpq_init();

    // Enable if needed for debugging
    if (0)
    {
        rdpq_debug_start();
        rdpq_debug_log(true);
    }

    int cur_demo_piece_i = 0;
    bool antialiasing_enabled = false;

    while (true)
    {
        struct DemoPiece *cur_demo_piece = &demo_pieces[cur_demo_piece_i];

        surface_t *disp = display_get();

        // Initialize libdragon OpenGL
        // Note: gl_init should typically be called only once, at boot, after rdpq_init
        //       This demo's main function specifically repeatedly closes and reinits the
        //       OpenGL context so that calling the demo piece callback works from a clean slate.
        gl_init();

        if (antialiasing_enabled)
        {
            glEnable(GL_MULTISAMPLE_ARB);
        }
        else
        {
            glDisable(GL_MULTISAMPLE_ARB);
        }

        cur_demo_piece->callback(disp);

        // matches gl_init
        // Note: you typically don't call gl_close, see note on the gl_init call above
        gl_close();

        // wait RSP and RDP done.
        // Note: In a real application, you should keep going with whatever processing the
        //       CPU can do and let it run in parallel with the RSP and RDP, and not wait on them.
        //       This wait is used here to then draw text on the framebuffer using
        //       graphics_draw_text, which is CPU drawing.
        //       I could also have used rdpq_font_print to make the RDP draw the text instead of the
        //       CPU, but I would need to provide a font and I am lazy.
        rspq_wait();

        // Draw text to indicate the demo piece and settings

        char msg[100];

        snprintf(msg, sizeof(msg), "%d/%d %s",
                 1 + cur_demo_piece_i, n_demo_pieces, cur_demo_piece->name);
        graphics_draw_text(disp, 20, 20, msg);

        snprintf(msg, sizeof(msg), "AA: %s", antialiasing_enabled ? "ON" : "OFF");
        graphics_draw_text(disp, 20, 30, msg);

        // The framebuffer is ready for displaying (not drawing anything else)
        // Queue the framebuffer for displaying on the next retrace
        display_show(disp);

        // Loop until the user switches the demo piece or changes settings.
        // The last displayed framebuffer will be displayed again every retrace
        //   ("the image will be frozen").
        int next_demo_piece_i = cur_demo_piece_i;
        bool redraw_demo_piece = false;
        while (next_demo_piece_i == cur_demo_piece_i && !redraw_demo_piece)
        {
            struct controller_data controller_data;

            controller_scan();
            controller_data = get_keys_down();

            if (controller_data.c[0].up || controller_data.c[0].C_up)
            {
                next_demo_piece_i -= 1;
            }
            if (controller_data.c[0].down || controller_data.c[0].C_down)
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

            if (controller_data.c[0].A)
            {
                antialiasing_enabled = !antialiasing_enabled;
                redraw_demo_piece = true;
            }

            wait_ms(1);
        }
        cur_demo_piece_i = next_demo_piece_i;
    }
}

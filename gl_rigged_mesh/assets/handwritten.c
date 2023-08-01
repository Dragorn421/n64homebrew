#include <GL/gl.h>

#include "handwritten.h"

void model_hourglass(void)
{
    GLfloat f = 0.8f;

    glEnable(GL_MATRIX_PALETTE_ARB);

    glMatrixMode(GL_MATRIX_PALETTE_ARB);

    glCurrentPaletteMatrixARB(0);
    glLoadIdentity();
    glScalef(0.5, 0.5, 0.5);

    glCurrentPaletteMatrixARB(1);
    glLoadIdentity();
    glTranslatef(0.3,0,0);

    glCurrentPaletteMatrixARB(2);
    glLoadIdentity();
    glRotatef(30, 0, 0, 1);

    glBegin(GL_TRIANGLES);

    glMatrixIndexubvARB(1, (GLubyte[]){0});
    glColor3ub(255, 0, 0);
    glVertex3f(f, f, 0);
    glVertex3f(-f, f, 0);
    glMatrixIndexubvARB(1, (GLubyte[]){1});
    glColor3ub(255, 255, 0);
    glVertex3f(0, 0, 0);

    glColor3ub(255, 255, 0);
    glVertex3f(0, 0, 0);
    glMatrixIndexubvARB(1, (GLubyte[]){2});
    glColor3ub(0, 255, 0);
    glVertex3f(f, -f, 0);
    glVertex3f(-f, -f, 0);

    glEnd();

    glDisable(GL_MATRIX_PALETTE_ARB);
}

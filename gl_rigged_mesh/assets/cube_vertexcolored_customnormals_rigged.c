#include <GL/gl.h>

void model_Cube(void)
{
    glBegin(GL_TRIANGLES);
    // tri #0
        // corner #0
            glNormal3f(0.8940784335136414, -0.3999463617801666, 0.20165973901748657);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #0s
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, -1.0, 0.0);
        // corner #1
            glNormal3f(0.9050058126449585, -0.404882550239563, -0.13051646947860718);
            glColor4f(1.0, 0.0018211620626971126, 0.0018211620626971126, 1.0);
            // vertex #1
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, -1.0, 2.0);
        // corner #2
            glNormal3f(0.7696410417556763, -0.6287598013877869, -0.11096743494272232);
            glColor4f(1.0, 0.009721217676997185, 0.009721217676997185, 1.0);
            // vertex #3
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, 1.0, 2.0);
    // tri #1
        // corner #0
            glNormal3f(0.8940784335136414, -0.3999463617801666, 0.20165973901748657);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #0
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, -1.0, 0.0);
        // corner #2
            glNormal3f(0.7696410417556763, -0.6287598013877869, -0.11096743494272232);
            glColor4f(1.0, 0.009721217676997185, 0.009721217676997185, 1.0);
            // vertex #3
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, 1.0, 2.0);
        // corner #3
            glNormal3f(0.7628956437110901, -0.6232162714004517, 0.1720224767923355);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #2
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, 1.0, 0.0);
    // tri #2
        // corner #4
            glNormal3f(0.762852132320404, -0.6232647895812988, 0.17203956842422485);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #2
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, 1.0, 0.0);
        // corner #5
            glNormal3f(0.7696409225463867, -0.6287602782249451, -0.1109657883644104);
            glColor4f(1.0, 0.009721217676997185, 0.009721217676997185, 1.0);
            // vertex #3
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, 1.0, 2.0);
        // corner #6
            glNormal3f(0.6049802303314209, -0.7841276526451111, -0.13835743069648743);
            glColor4f(1.0, 0.004776953253895044, 0.004776953253895044, 1.0);
            // vertex #7
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, 1.0, 2.0);
    // tri #3
        // corner #4
            glNormal3f(0.762852132320404, -0.6232647895812988, 0.17203956842422485);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #2
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, 1.0, 0.0);
        // corner #6
            glNormal3f(0.6049802303314209, -0.7841276526451111, -0.13835743069648743);
            glColor4f(1.0, 0.004776953253895044, 0.004776953253895044, 1.0);
            // vertex #7
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, 1.0, 2.0);
        // corner #7
            glNormal3f(0.5967637896537781, -0.7734835743904114, 0.21353264153003693);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #6
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, 1.0, 0.0);
    // tri #4
        // corner #8
            glNormal3f(0.5967539548873901, -0.7734990119934082, 0.21350406110286713);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #6
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, 1.0, 0.0);
        // corner #9
            glNormal3f(0.6049544215202332, -0.7841417789459229, -0.13839025795459747);
            glColor4f(1.0, 0.004776953253895044, 0.004776953253895044, 1.0);
            // vertex #7
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, 1.0, 2.0);
        // corner #10
            glNormal3f(0.801679790019989, -0.5689246654510498, -0.18339647352695465);
            glColor4f(1.0, 0.0003035269910469651, 0.0, 1.0);
            // vertex #5
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, -1.0, 2.0);
    // tri #5
        // corner #8
            glNormal3f(0.5967539548873901, -0.7734990119934082, 0.21350406110286713);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #6
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, 1.0, 0.0);
        // corner #10
            glNormal3f(0.801679790019989, -0.5689246654510498, -0.18339647352695465);
            glColor4f(1.0, 0.0003035269910469651, 0.0, 1.0);
            // vertex #5
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, -1.0, 2.0);
        // corner #11
            glNormal3f(0.7828764915466309, -0.5555518269538879, 0.2801187336444855);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #4
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, -1.0, 0.0);
    // tri #6
        // corner #12
            glNormal3f(0.7828529477119446, -0.5555851459503174, 0.28011849522590637);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #4
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, -1.0, 0.0);
        // corner #13
            glNormal3f(0.8016782402992249, -0.5689444541931152, -0.18334178626537323);
            glColor4f(1.0, 0.0003035269910469651, 0.0, 1.0);
            // vertex #5
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, -1.0, 2.0);
        // corner #14
            glNormal3f(0.9050211906433105, -0.40485846996307373, -0.13048473000526428);
            glColor4f(1.0, 0.0018211620626971126, 0.0018211620626971126, 1.0);
            // vertex #1
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, -1.0, 2.0);
    // tri #7
        // corner #12
            glNormal3f(0.7828529477119446, -0.5555851459503174, 0.28011849522590637);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #4
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, -1.0, 0.0);
        // corner #14
            glNormal3f(0.9050211906433105, -0.40485846996307373, -0.13048473000526428);
            glColor4f(1.0, 0.0018211620626971126, 0.0018211620626971126, 1.0);
            // vertex #1
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, -1.0, 2.0);
        // corner #15
            glNormal3f(0.8940853476524353, -0.39994320273399353, 0.20163549482822418);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #0
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, -1.0, 0.0);
    // tri #8
        // corner #16
            glNormal3f(0.7628525495529175, -0.623273491859436, 0.17200618982315063);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #2
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, 1.0, 0.0);
        // corner #17
            glNormal3f(0.5967338681221008, -0.7735179662704468, 0.2134915143251419);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #6
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, 1.0, 0.0);
        // corner #18
            glNormal3f(0.782856285572052, -0.5555931925773621, 0.2800931930541992);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #4
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, -1.0, 0.0);
    // tri #9
        // corner #16
            glNormal3f(0.7628525495529175, -0.623273491859436, 0.17200618982315063);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #2
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, 1.0, 0.0);
        // corner #18
            glNormal3f(0.782856285572052, -0.5555931925773621, 0.2800931930541992);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #4
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(1.0, -1.0, 0.0);
        // corner #19
            glNormal3f(0.8940639495849609, -0.3999713957309723, 0.20167407393455505);
            glColor4f(1.0, 1.0, 1.0, 1.0);
            // vertex #0
                glMatrixIndexubvARB(1, (GLubyte[]){0});
                glVertex3f(-1.0, -1.0, 0.0);
    // tri #10
        // corner #20
            glNormal3f(0.6049434542655945, -0.7841597199440002, -0.13833646476268768);
            glColor4f(1.0, 0.004776953253895044, 0.004776953253895044, 1.0);
            // vertex #7
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, 1.0, 2.0);
        // corner #21
            glNormal3f(0.769614040851593, -0.6287977695465088, -0.11094000935554504);
            glColor4f(1.0, 0.009721217676997185, 0.009721217676997185, 1.0);
            // vertex #3
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, 1.0, 2.0);
        // corner #22
            glNormal3f(0.9050140380859375, -0.4048701524734497, -0.1304984986782074);
            glColor4f(1.0, 0.0018211620626971126, 0.0018211620626971126, 1.0);
            // vertex #1
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, -1.0, 2.0);
    // tri #11
        // corner #20
            glNormal3f(0.6049434542655945, -0.7841597199440002, -0.13833646476268768);
            glColor4f(1.0, 0.004776953253895044, 0.004776953253895044, 1.0);
            // vertex #7
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, 1.0, 2.0);
        // corner #22
            glNormal3f(0.9050140380859375, -0.4048701524734497, -0.1304984986782074);
            glColor4f(1.0, 0.0018211620626971126, 0.0018211620626971126, 1.0);
            // vertex #1
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(-1.0, -1.0, 2.0);
        // corner #23
            glNormal3f(0.8016772866249084, -0.5689501762390137, -0.18332821130752563);
            glColor4f(1.0, 0.0003035269910469651, 0.0, 1.0);
            // vertex #5
                glMatrixIndexubvARB(1, (GLubyte[]){1});
                glVertex3f(1.0, -1.0, 2.0);
    glEnd();
}

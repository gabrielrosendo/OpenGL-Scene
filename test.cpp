#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
void displayMe(void)
{
    // Set the background color to blue sky (R, G, B, Alpha)
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue color
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f); // White color

    glBegin(GL_QUADS); // Use GL_LINE_LOOP for an unfilled rectangle
        glVertex3f(0.5, 0.0, 0.0); // (0.0, 0.5, 0.0)  +--------+  (0.5, 0.5, 0.0)
        glVertex3f(0.5, 1.0, 0.0); //                  |        |
        glVertex3f(0.0, 1.0, 0.0); //                  |        |
        glVertex3f(0.0, 0.0, 0.0); //(0.0, 0.0, 0.0)   +--------+  (0.5, 0.0, 0.0)
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scene One");
    glutDisplayFunc(displayMe);
    glutMainLoop();
    return 0;
}


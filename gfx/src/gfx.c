#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

int init_resources(void)
{
    // XXX
    return 1;
}

void onDisplay()
{
    // XXX
}

void free_resources()
{
    // XXX
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("GFX");

    GLenum glew_status = glewInit();

    if (glew_status != GLEW_OK)
    {
        fprintf(stderr, "Error: %s", glewGetErrorString(glew_status));
        return EXIT_FAILURE;
    }

    if (1 == init_resources())
    {
        glutDisplayFunc(onDisplay);
        glutMainLoop();
    }

    free_resources();
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <shader_utils.h>

GLuint program;
GLint attribute_coord2d;
GLuint vbo_triangle;

int init_resources(void)
{
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    GLuint vs, fs;

    if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER)) == 0)
    {
        return 0;
    }
    if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0)
    {
        return 0;
    }

    GLfloat triangle_vertices[] = {
         0.0,  0.8,
        -0.8, -0.8,
         0.8, -0.8,
    };

    glGenBuffers(1, &vbo_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices,
            GL_STATIC_DRAW);

    // Program
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);

    if (!link_ok)
    {
        fprintf(stderr, "glLinkProgram:");
        print_log(program);
        return 0;
    }

    const char *attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);

    if (attribute_coord2d == -1)
    {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }

    return 1;
}

void onDisplay()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
    glEnableVertexAttribArray(attribute_coord2d);

    glVertexAttribPointer(
        attribute_coord2d,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);

    glutSwapBuffers();
}

void free_resources()
{
    glDeleteProgram(program);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
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

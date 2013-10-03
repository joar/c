#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <shader_utils.h>

GLuint program;
GLint attribute_coord2d, attribute_v_color;
GLuint vbo_triangle;

struct attributes {
    GLfloat coord2d[2];
    GLfloat v_color[3];
};

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

    //
    // Buffers and attributes
    //

    struct attributes triangle_attributes[] = {
        {{ 0.0,  0.8},     {1.0, 1.0, 0.0}},
        {{-0.8, -0.8},     {0.0, 0.0, 1.0}},
        {{ 0.8, -0.8},     {1.0, 0.0, 0.0}},
    };

    glGenBuffers(1, &vbo_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(triangle_attributes),
            triangle_attributes,
            GL_STATIC_DRAW
    );

    const char *attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);

    if (attribute_coord2d == -1)
    {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }

    attribute_name = "v_color";
    attribute_v_color = glGetAttribLocation(program, attribute_name);

    if (attribute_v_color == -1)
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

    glEnableVertexAttribArray(attribute_coord2d);
    glEnableVertexAttribArray(attribute_v_color);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

    glVertexAttribPointer(
        attribute_coord2d,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(struct attributes),
        0
    );

    glVertexAttribPointer(
        attribute_v_color,  // attribute
        3,                  // number of elements per vertex
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        sizeof(struct attributes),// stride
        (GLvoid *) offsetof(struct attributes, v_color)// offset
    );

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);
    glDisableVertexAttribArray(attribute_v_color);

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

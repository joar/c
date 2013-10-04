#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_utils.hpp>
#include <res_texture.c>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int screen_width = 800, screen_height = 600;

GLuint program;

GLint attribute_coord3d, attribute_v_color, attribute_texcoord;

GLuint vbo_cube_vertices, vbo_cube_colors, vbo_cube_texcoords;
GLuint ibo_cube_elements;

GLint uniform_mvp;

GLuint texture_id;
GLint uniform_mytexture;

int init_resources(void)
{

    //
    // Buffers and attributes
    //

    GLfloat cube_vertices[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // top
        -1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
        // bottom
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,
        // left
        -1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        // right
         1.0, -1.0,  1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
         1.0,  1.0,  1.0,
    };

    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
            GL_STATIC_DRAW);

    GLfloat cube_colors[] = {
        // front colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
    };

    glGenBuffers(1, &vbo_cube_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors,
            GL_STATIC_DRAW);

    GLushort cube_elements[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        4, 5, 6,
        6, 7, 4,
        // back
        8, 9, 10,
        10, 11, 8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20,
    };

    glGenBuffers(1, &ibo_cube_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements,
            GL_STATIC_DRAW);

    GLfloat cube_texcoords[2 * 4 * 6] = {
        // front
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
    };

    for (int i = 1; i < 6; i++)
    {
        memcpy(
                &cube_texcoords[i * 4 * 2],
                &cube_texcoords[0],
                2 * 4 * sizeof(GLfloat)
        );
    }

    glGenBuffers(1, &vbo_cube_texcoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords,
            GL_STATIC_DRAW);

    //
    // Textures
    //

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            res_texture.width,
            res_texture.height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            res_texture.pixel_data
    );

    //
    // Shaders
    //

    GLint link_ok = GL_FALSE;

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
    // Attributes
    //

    const char *attribute_name = "coord3d";
    attribute_coord3d = glGetAttribLocation(program, attribute_name);

    if (attribute_coord3d == -1)
    {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }

    attribute_name = "texcoord";
    attribute_texcoord = glGetAttribLocation(program, attribute_name);
    if (attribute_texcoord == -1)
    {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }

    //
    // Uniforms
    //
    const char *uniform_name;

    uniform_name = "mvp";
    uniform_mvp = glGetUniformLocation(program, uniform_name);
    if (uniform_mvp == -1)
    {
        fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
        return 0;
    }

    uniform_name = "mytexture";
    uniform_mytexture = glGetUniformLocation(program, uniform_name);
    if (uniform_mytexture == -1)
    {
        fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
        return 0;
    }

    return 1;
}

void onDisplay()
{
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    // Vertices

    glEnableVertexAttribArray(attribute_coord3d);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);

    glVertexAttribPointer(
        attribute_coord3d,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );

    // Color

    glEnableVertexAttribArray(attribute_v_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);

    glVertexAttribPointer(
        attribute_v_color,  // attribute
        3,                  // number of elements per vertex
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0, // stride
        0 // offset
    );

    // Texture coordinates

    glEnableVertexAttribArray(attribute_texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glVertexAttribPointer(
            attribute_texcoord,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            0
    );

    // Texture

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(uniform_mytexture, 0);

    // Elements

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);

    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_v_color);
    glutSwapBuffers();
}

void onReshape(int width, int height)
{
    fprintf(stderr, "reshape: %dx%d\n", width, height);
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, screen_width, screen_height);
}

void onIdle()
{
    glm::mat4 model = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 view = glm::lookAt(
            glm::vec3(0.0, 2.0, 0.0),
            glm::vec3(0.0, 0.0, -4.0),
            glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 projection = glm::perspective(
            45.0f,
            1.0f * screen_width / screen_height,
            0.1f,
            10.0f);

    float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 15;
    glm::mat4 anim = \
         glm::rotate(glm::mat4(1.0f), angle * 3.0f, glm::vec3(1, 0, 0)) * // X
         glm::rotate(glm::mat4(1.0f), angle * 2.0f, glm::vec3(0, 1, 0)) * // Y
         glm::rotate(glm::mat4(1.0f), angle * 4.0f, glm::vec3(0, 0, 1));  // Z

    glm::mat4 mvp = projection * view * model * anim;

    glUseProgram(program);

    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    glutPostRedisplay();
}

void free_resources()
{
    glDeleteProgram(program);
    glDeleteTextures(1, &texture_id);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(screen_width, screen_height);
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
        glutIdleFunc(onIdle);

        glutReshapeFunc(onReshape);

        glutMainLoop();
    }

    free_resources();
    return EXIT_SUCCESS;
}

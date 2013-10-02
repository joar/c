#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

GLuint program;
GLint attribute_coord2d;

char* file_read(const char *filename)
{
    FILE* input = fopen(filename, "rb");

    if (input == NULL)
    {
        return NULL;
    }
    if (fseek(input, 0, SEEK_END) == -1)
    {
        return NULL;
    }

    long size = ftell(input);
    if (size == -1)
    {
        return NULL;
    }

    char *content = (char*) malloc((size_t) size +1);
    if (content == NULL)
    {
        return NULL;
    }
    fread(content, 1, (size_t)size, input);
    if (ferror(input))
    {
        free(content);
        return NULL;
    }
    fclose(input);
    content[size] = '\0';
    return content;
}

void print_log(GLuint object)
{
    GLint log_length = 0;

    if (glIsShader(object))
    {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }
    else if (glIsProgram(object))
    {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }
    else
    {
        fprintf(stderr, "print_log: Not a shader or a program\n");
        return;
    }

    char *log = (char*)malloc(log_length);

    if (glIsShader(object))
    {
        glGetShaderInfoLog(object, log_length, NULL, log);
    }
    else if (glIsProgram(object))
    {
        glGetProgramInfoLog(object, log_length, NULL, log);
    }
    fprintf(stderr, "%s", log);
    free(log);
}

int init_resources(void)
{
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    // Vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_source =
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"  // OpenGL ES 2.0
#else
        "#version 120\n"  // OpenGL 2.1
#endif
        "attribute vec2 coord2d;"
        "void main(void) {"
        "   gl_Position = vec4(coord2d, 0.0, 1.0);"
        "}";
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (0 == compile_ok)
    {
        fprintf(stderr, "Error in vertex shader\n");
        return 0;
    }

    // Fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_source =
        "#version 120\n"
        "void main(void) {"
        "   gl_FragColor[0] = 0.0;"
        "   gl_FragColor[1] = 0.0;"
        "   gl_FragColor[2] = 1.0;"
        "}";

    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok)
    {
        fprintf(stderr, "Error in fragment shader\n");
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
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    GLfloat triangle_vertices[] = {
         0.0,  0.8,
        -0.8, -0.8,
         0.8, -0.8
    };

    glVertexAttribPointer(
        attribute_coord2d,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        triangle_vertices
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

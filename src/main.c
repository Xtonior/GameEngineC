#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

#include "window.h"
#include "shader.h"
#include "mesh.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char *WINDOW_TITLE = "Engine";

GLenum err;

GLFWwindow *window;

Shader *main_shader;

Mesh *triangleMesh;

float vertices[] = {
    // positions         // colors
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
};

int init()
{
    window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    if (initWindow(window) == -1)
    {
        printf("Failed to create GLFW window");
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }

    main_shader = shaderCreate("assets/shaders/vertex.vs", "assets/shaders/fragment.fs");

    return 0;
}

void createObjects()
{
    unsigned int numVertices = sizeof(vertices) / sizeof(vertices)[0];
    triangleMesh = genMesh(&vertices, numVertices);
}

void gameLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderUse(main_shader);
        renderMesh(triangleMesh);
        updateWindow(window);

        while ((err = glGetError()) != GL_NO_ERROR)
        {
            printf("OpenGL error: %d\n", err);
        }
    }
}

void terminate()
{
    terminateWindow(window);
    deleteMesh(triangleMesh);
}

int main()
{
    if (init() == -1)
    {
        printf("Failed init");
    }

    createObjects();
    gameLoop();
    terminate();

    return 0;
}
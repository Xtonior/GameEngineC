#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdbool.h>

#include "window.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char *WINDOW_TITLE = "Engine";

float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLenum err;

GLFWwindow *window;

Shader *main_shader;

Mesh *triangleMesh;

Camera *mainCamera;

float vertices[] = {
    // positions         // colors
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
};

void cameraInput(float dt)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ProcessKeyboard(mainCamera, FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ProcessKeyboard(mainCamera, BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ProcessKeyboard(mainCamera, LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ProcessKeyboard(mainCamera, RIGHT, dt);
}

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

    mainCamera = CreateCamera((vec3){0.0f, 5.0f, 0.0f}, 10.0f, 0.1f, (vec3){1.0f, 0.0f, 0.0f});
}

void gameLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        cameraInput(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderUse(main_shader);

        mat4 cameraView;
        mat4 cameraProjection;
        glm_mat4_identity(cameraView);
        glm_mat4_identity(cameraProjection);

        vec3 pos = {1.0f, 0.0f, 0.0f};
        mat4 trans;
        glm_mat4_identity(trans);
        glm_translate(trans, pos);
        renderMesh(triangleMesh, main_shader, trans);
        updateWindow(window);

        while ((err = glGetError()) != GL_NO_ERROR)
        {
            printf("OpenGL error: %d\n", err);
        }
    }
}

void terminate()
{
    deleteMesh(triangleMesh);
    terminateWindow(window);
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

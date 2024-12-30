#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdbool.h>

#include "cglm/affine-pre.h"
#include "cglm/cam.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "window.h"
#include "editorUi.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "framebuffer.h"

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

float mouseDeltaX, mouseDeltaY;

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

    getMouse(&mouseDeltaX, &mouseDeltaY);

    // ProcessMouseMovement(mainCamera, mouseDeltaX, mouseDeltaY, 1);
}

unsigned int vpmain_fbo;
unsigned int vpmain_rbo;
unsigned int vpmain_tex;

int initMainViewport()
{
    return createFramebuffer(&vpmain_fbo, &vpmain_rbo, &vpmain_tex, 600, 400);
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

    editorui_init(window);

    main_shader = shaderCreate("assets/shaders/vertex.vs", "assets/shaders/fragment.fs");

    if (initMainViewport() == -1) return -1;

    return 0;
}

void createObjects()
{
    unsigned int numVertices = sizeof(vertices) / sizeof(vertices)[0];
    triangleMesh = genMesh(vertices, numVertices);

    mainCamera = CreateCamera((vec3){0.0f, 0.0f, -2.0f}, 10.0f, 0.1f);
}

void renderScene()
{
    mat4 triModel;
    vec3 triPos = {0.0f, 0.0f, -3.0f};
    glm_mat4_identity(triModel);
    glm_translate(triModel, triPos);

    renderMesh(triangleMesh, main_shader, &triModel);
}

int vp_w = 150;
int vp_h = 70;

void renderMainViewport()
{
    bindFramebuffer(&vpmain_fbo);

    resizeFramebuffer(&vpmain_tex, &vpmain_rbo, vp_w, vp_h);
    // glViewport(0, 0, vp_w, vp_h); 
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    editorui_renderViewport(&vpmain_tex, &vp_w, &vp_h);
    renderScene();

    // Unbind framebuffer after rendering
    unbindFramebuffer();
}

void proceedCamera()
{
    shaderUse(main_shader);

    mat4 cameraProjection;
    glm_perspective(glm_rad(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f, cameraProjection);
    mat4 cameraView;
    GetViewMatrix(mainCamera, &cameraView);
    shaderSetMat4(main_shader, "projection", &cameraProjection);
    shaderSetMat4(main_shader, "view", &cameraView);
}

void gameLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);

        editorui_update();

        float currentFrame = (float)(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        cameraInput(deltaTime);
        ProcessMouseMovement(mainCamera, 0.0f, 0.0f, 1);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        proceedCamera();
        // renderScene();
        renderMainViewport();
        editorui_render(window);
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
    editorui_terminate();
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
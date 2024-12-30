#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#include "window.h"

bool firstMouse = true;
bool showCursor = false;

float lastX, lastY;
float xoffset, yoffset;
float xpos, ypos;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

GLFWwindow *createWindow(int w, int h, const char *title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(w, h, title, NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

int initWindow(GLFWwindow *window)
{
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    return 0;
}

void updateWindow(GLFWwindow *window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (showCursor == 1)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else if (showCursor == 0)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void getMouse(float *offsetX, float *offsetY)
{
    *offsetX = xoffset;
    *offsetY = yoffset;
}

// Callbacks

/*void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = (float)(xposIn);
    float ypos = (float)(yposIn);

    if (firstMouse == true)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xdelta = xpos - lastX;
    float ydelta = lastY - ypos; // reversed since y-coordinates go from bottom to top

    // printf("LP: %f, %f\n", lastX, lastY);
    // printf("OF: %f, %f\n", xoffset, yoffset);
    printf("OF: %f - %f = %f\n", xpos, lastX, xdelta);

    lastX = xpos;
    lastY = ypos;
}*/

void updateMouse(GLFWwindow *window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    xpos = (float)x;
    ypos = (float)y;

    if (firstMouse == true)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    xoffset = xpos - lastX;
    yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        showCursor = !showCursor;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    updateMouse(window);
}

void terminateWindow(GLFWwindow *window)
{
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

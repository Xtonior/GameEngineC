#include <GLFW/glfw3.h>

#ifndef WINDOW_H
#define WINDOW_H

GLFWwindow *createWindow(int w, int h, const char *title);
int initWindow(GLFWwindow *window);
void updateWindow(GLFWwindow *window);
void processInput(GLFWwindow *window);
void terminateWindow(GLFWwindow *window);

#endif
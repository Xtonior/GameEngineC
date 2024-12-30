#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "cglm/cam.h"
#include "cglm/types.h"
#include "mesh.h"
#include "shader.h"

Shader *shader;
vec3 pos = {0.0f, 0.0f, 0.0f};
mat4 proj;

int viewport_init()
{
    return 0;
}

void viewport_createOrtho()
{
    shader = shaderCreate("assets/shaders/grid.vs", "assets/shaders/grid.fs");
    glm_ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, proj);
}

void viewport_renderOrtho(Mesh *mesh, mat4 *model, int w, int h)
{
    shaderUse(shader);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm_perspective(glm_rad(60.0f), (float)w / (float)h, 0.1f, 100.0f, proj);

    shaderSetMat4(shader, "projection", &proj);
    shaderSetMat4(shader, "model", model);

    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices);

    glBindVertexArray(0);
}

#endif
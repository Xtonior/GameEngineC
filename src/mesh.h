#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <cglm/types.h>

#include <stdlib.h>
#include <stdio.h>

#include "shader.h"

typedef struct Mesh
{
    unsigned int numVertices;
    float *vertices;
    unsigned int VBO, VAO;
} Mesh;

Mesh *genMesh(float *vertices, unsigned int numVertices)
{
    Mesh *mesh = (Mesh *)malloc(sizeof(Mesh));

    if (mesh == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    mesh->vertices = vertices;
    mesh->numVertices = numVertices;

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return mesh;
}

void renderMesh(Mesh *mesh, Shader *shader, mat4 transform)
{
    unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform[0]);

    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices);
}

void deleteMesh(Mesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
}

#endif
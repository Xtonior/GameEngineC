#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Shader
{
    unsigned int ID;
} Shader;

// Utility function to read a file into a string
char *readFile(const char *filePath)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(filePath, "rb");

    if (!f)
    {
        fprintf(stderr, "Can't find shader file: %s\n", filePath);
        return 0;
    }

    fseek(f, 0, SEEK_END);
    length = ftell(f);

    fseek(f, 0, SEEK_SET);
    buffer = (char *)malloc(length + 1);

    if (!buffer)
    {
        fprintf(stderr, "Can't read shader file: %s\n", filePath);
        return 0;
    }

    fread(buffer, 1, length, f);

    fclose(f);
    buffer[length] = '\0';

    return buffer;
}

// Utility function for checking shader compilation/linking errors
static void checkCompileErrors(unsigned int shader, const char *type)
{
    int success;
    char infoLog[1024];

    if (strcmp(type, "PROGRAM") != 0)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
}

// Initialize a shader
Shader *shaderCreate(const char *vertexPath, const char *fragmentPath)
{
    Shader *shader = (Shader *)malloc(sizeof(Shader));
    if (!shader)
    {
        fprintf(stderr, "ERROR::SHADER::MEMORY_ALLOCATION_FAILED\n");
        return NULL;
    }

    char *vertexCode = readFile(vertexPath);
    char *fragmentCode = readFile(fragmentPath);

    if (!vertexCode || !fragmentCode)
    {
        free(vertexCode);
        free(fragmentCode);
        free(shader);
        return NULL;
    }

    const char *vShaderCode = vertexCode;
    const char *fShaderCode = fragmentCode;

    // Compile vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Compile fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Create shader program
    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);
    checkCompileErrors(shader->ID, "PROGRAM");

    // Delete shaders as they are no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertexCode);
    free(fragmentCode);

    return shader;
}

// Use the shader program
void shaderUse(const Shader *shader)
{
    glUseProgram(shader->ID);
}

// Set uniform functions
void shaderSetBool(const Shader *shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shaderSetInt(const Shader *shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shaderSetFloat(const Shader *shader, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

// Cleanup shader
void shaderDestroy(Shader *shader)
{
    if (shader)
    {
        glDeleteProgram(shader->ID);
        free(shader);
    }
}

#endif

#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/cam.h"
#include "cglm/vec3.h"
#include <cglm/cglm.h>
#include <math.h>
#include <stdlib.h>

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

const float DEFAULT_YAW 			= -90.0f;
const float DEFAULT_PITCH 			= 0.0f;
const float DEFAULT_SPEED 			= 2.5f;
const float DEFAULT_SENSITIVITY 	= 0.1f;

typedef struct Camera
{
	// Camera attributes
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;

	// euler angles
	float Yaw;
	float Pitch;

	// Camera options
	float movementSpeed;
	float mouseSensitivity;
} Camera;

Camera *CreateCamera(vec3 pos, float speed, float sens)
{
	Camera *cam = malloc(sizeof(Camera));

	glm_vec3_copy(pos, cam->Position);

	cam->movementSpeed = speed;
	cam->mouseSensitivity = sens;
	
	cam->Pitch = DEFAULT_PITCH;
	cam->Yaw = DEFAULT_YAW;

	glm_vec3_copy((vec3){0.0, 1.0, 0.0}, cam->WorldUp);

	return cam;
}

void UpdateCameraVectors(Camera *camera)
{
	vec3 front;
	vec3 right;
	vec3 up;

	front[0] = cos(glm_rad(camera->Pitch)) * cos(glm_rad(camera->Yaw));  
    front[1] = sin(glm_rad(camera->Pitch));                               
    front[2] = cos(glm_rad(camera->Pitch)) * sin(glm_rad(camera->Yaw));  

	glm_normalize(front);
	glm_vec3_copy(front, camera->Front);

	glm_cross(camera->Front, camera->WorldUp, right);
	glm_normalize(right);
	glm_vec3_copy(right, camera->Right);

	glm_cross(camera->Right, camera->Front, up);
	glm_normalize(up);
	glm_vec3_copy(up, camera->Up);
}


void GetViewMatrix(Camera *camera, mat4 *m)
{
	vec3 v;

	glm_vec3_add(camera->Position, camera->Front, v);
	glm_lookat(camera->Position, v, camera->Up, *m);
}

void ProcessKeyboard(Camera *camera, enum CameraMovement direction, float deltaTime)
{
    float velocity = camera->movementSpeed * deltaTime;  

    vec3 vel = GLM_VEC3_ZERO_INIT;
	vec3 newPos = GLM_VEC3_ZERO_INIT;

    if (direction == FORWARD)
    {
		glm_vec3_copy((vec3){velocity, velocity, velocity}, vel);
        glm_vec3_mul(camera->Front, vel, newPos);

		glm_vec3_add(camera->Position, newPos, camera->Position);
    }
    if (direction == BACKWARD)
    {
        glm_vec3_copy((vec3){velocity, velocity, velocity}, vel);
        glm_vec3_mul(camera->Front, vel, newPos);

		glm_vec3_negate(newPos);

		glm_vec3_add(camera->Position, newPos, camera->Position);
    }
    if (direction == LEFT)
    {
        glm_vec3_copy((vec3){velocity, velocity, velocity}, vel);
        glm_vec3_mul(camera->Right, vel, newPos);

		glm_vec3_negate(newPos);

		glm_vec3_add(camera->Position, newPos, camera->Position);
    }
    if (direction == RIGHT)
    {
        glm_vec3_copy((vec3){velocity, velocity, velocity}, vel);
        glm_vec3_mul(camera->Right, vel, newPos);

		glm_vec3_add(camera->Position, newPos, camera->Position);
    }
}


void ProcessMouseMovement(Camera *camera, float xdelta, float ydelta, int constrainPitch)
{
	xdelta *= camera->mouseSensitivity;
	ydelta *= camera->mouseSensitivity;

	camera->Yaw += xdelta;
	camera->Pitch += ydelta;

	if (constrainPitch == 1)
    {
        if (camera->Pitch > 89.0f)
            camera->Pitch = 89.0f;
        if (camera->Pitch < -89.0f)
            camera->Pitch = -89.0f;
    }

	UpdateCameraVectors(camera);
}

#endif

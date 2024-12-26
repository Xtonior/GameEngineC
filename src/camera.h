#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/cam.h"
#include "cglm/vec3.h"
#include <cglm/cglm.h>
#include <math.h>

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

Camera *CreateCamera(vec3 pos, float speed, float sens, vec3 front)
{
	Camera *cam;

	glm_vec3_copy(pos, cam->Position);
	cam->movementSpeed = speed;
	cam->mouseSensitivity = sens;
	glm_vec3_copy(front, cam->Front);

	return cam;
}

void UpdateCameraVectors(Camera *camera)
{
	vec3 front;
	vec3 right;
	vec3 up;

	front[0] = cos(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch));
	front[1] = sin(glm_rad(camera->Pitch));
	front[2] = sin(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch));

	glm_normalize(front);
	glm_vec3_copy(front, camera->Front);

	glm_cross(camera->Front, camera->WorldUp, right);
	glm_normalize(right);

	glm_cross(camera->Right, camera->Front, up);
	glm_normalize(up);
}

mat4 *GetViewMatrix(Camera *camera)
{
	vec3 v;
	mat4 *m;

	glm_vec3_add(camera->Position, camera->Front, v);
	glm_lookat(camera->Position, v, camera->Up, m);
	return m;
}

void ProcessKeyboard(Camera *camera, enum CameraMovement direction, float deltaTime)
{
    vec3 velocity = {camera->movementSpeed * deltaTime};

	vec3 v;

    if (direction == FORWARD)
		glm_vec3_mulv(camera->Front, velocity, v);
    if (direction == BACKWARD)
	{
		glm_vec3_mulv(camera->Front, velocity, v);
		glm_vec3_negate_to(velocity, v);
	}
    if (direction == LEFT)
	{
		glm_vec3_mulv(camera->Right, velocity, v);
		glm_vec3_negate_to(velocity, v);
	}
    if (direction == RIGHT)
	{
		glm_vec3_mulv(camera->Right, velocity, v);
	}

	glm_vec3_add(camera->Position, v, camera->Position);
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

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
Camera::Camera()
{}

Camera::~Camera()
{}

void Camera::Init()
{
	CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	CameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	UpdateCameraVector();
}

void Camera::UpdateCameraVector()
{
	glm::vec3 Target;
	Target.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Target.y = sin(glm::radians(Pitch));
	Target.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	CameraTarget = glm::normalize(Target);
	CameraRight = glm::normalize(glm::cross(CameraTarget, WorldUp));// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraTarget));
}

void Camera::CalculateCameraDirection(Camera_Direction direction, float deltaTime)
{
	float velocity = CameraMovementSpeed * deltaTime;
	if (direction == FORWARD)
		CameraPosition += CameraTarget * velocity;
	
	if (direction == BACKWARD)
		CameraPosition -= CameraTarget * velocity;
	
	if (direction == RIGHT)
		CameraPosition += CameraRight * velocity;
		
	if (direction == LEFT)
		CameraPosition -= CameraRight * velocity;

	if (direction == UP)
		CameraPosition += CameraUp * velocity;
		
	if (direction == DOWN)
		CameraPosition -= CameraUp * velocity;
}

void Camera::CalculateMouseMovement(float xoffset, float yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
	UpdateCameraVector();
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(Fov), AspectRatio, Near, Far);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(CameraPosition, CameraPosition + CameraTarget, CameraUp);
}
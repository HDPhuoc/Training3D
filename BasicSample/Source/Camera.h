#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"

enum Camera_Direction {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	glm::vec3 CameraPosition;
	glm::vec3 CameraTarget;
	glm::vec3 WorldUp;
	glm::vec3 CameraUp;
	glm::vec3 CameraRight;
	Camera();
	~Camera();
	void Init();
	void UpdateCameraVector();
	void CalculateCameraDirection(Camera_Direction direction, float deltaTime);
	void CalculateMouseMovement(float xoffset, float yoffset);
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
private:
	float AspectRatio = (float)(1480 / 720);
	float Near = 0.1f;
	float Far = 100.f;
	float Fov = 45.0f;
	float CameraMovementSpeed = 2.5f;
	float Pitch = 0.0f;
	float Yaw = -90.0f;
	float MouseSensitivity = 0.1f;
};
#endif /* CAMERA_H */

//#include <iostream>
//#include "glm/glm.hpp"	
//#include "glm/gtc/matrix_transform.hpp"
//#include <glm/gtc/type_ptr.hpp>
//
//class Camera
//{
//public:
//	float AspectRatio;
//	float Near;
//	float Far;
//	float Fov;
//	float CameraMovementSpeed;
//	float Pitch;
//	float Yaw;
//	float MouseSensitivity;
//	//camera
//	glm::vec3 CameraPosition;
//	glm::vec3 CameraTarget;
//	glm::vec3 WorldUp;
//	glm::vec3 CameraUp;
//	glm::vec3 CameraRight;
//	Camera(glm::vec3 cameraPosition)
//		: Fov(45.0f)//For a realistic view it is usually set to 45 degrees, so i set it to init
//		, CameraMovementSpeed(2.5f)
//		, Near(0.1f)
//		, Far(100.f)
//		, Pitch(0.0f)
//		, Yaw (-90.0f)// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//		, MouseSensitivity(0.1f)
//	{
//		CameraPosition = cameraPosition;
//		CameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
//		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
////#ifdef _WIN32
//		AspectRatio = 1480 / 720;
////#else
//		//AspectRatio = 2560 / 1440;
////#endif // _WIN32
//		UpdateCameraVector();
//	}
//
//	//function
//	void CalculateCameraDirection(Camera_Direction direction, float deltaTime)
//	{
//		float velocity = CameraMovementSpeed * deltaTime;
//		if (direction == FORWARD) {
//			CameraPosition += CameraTarget * velocity;
//			std::cout << "move FORWARD CameraPosition.x=" << CameraPosition.x << " CameraPosition.y=" << CameraPosition.y << " CameraPosition.z=" << CameraPosition.z << std::endl;
//		}
//		if (direction == BACKWARD) {
//			CameraPosition -= CameraTarget * velocity;
//			std::cout << "move BACKWARD CameraPosition.x=" << CameraPosition.x << " CameraPosition.y=" << CameraPosition.y << " CameraPosition.z=" << CameraPosition.z << std::endl;
//		}
//		if (direction == RIGHT) {
//			CameraPosition += CameraRight * velocity;
//			std::cout << "move RIGHT CameraPosition.x=" << CameraPosition.x << " CameraPosition.y=" << CameraPosition.y << " CameraPosition.z=" << CameraPosition.z << std::endl;
//		}
//		if (direction == LEFT) {
//			CameraPosition -= CameraRight * velocity;
//			std::cout << "move LEFT CameraPosition.x=" << CameraPosition.x << " CameraPosition.y=" << CameraPosition.y << " CameraPosition.z=" << CameraPosition.z << std::endl;
//		}
//		if (direction == UP) {
//			CameraPosition += CameraUp * velocity;
//			std::cout << "move UP CameraPosition.x=" << CameraPosition.x << " CameraPosition.y=" << CameraPosition.y << " CameraPosition.z=" << CameraPosition.z << std::endl;
//		}
//		if (direction == DOWN) {
//			CameraPosition -= CameraUp * velocity;
//			std::cout << "move DOWN CameraPosition.x=" << CameraPosition.x << " CameraPosition.y=" << CameraPosition.y << " CameraPosition.z=" << CameraPosition.z << std::endl;
//		}
//	}
//	void CalculateMouseMovement(float xoffset, float yoffset)
//	{
//		xoffset *= MouseSensitivity;
//		yoffset *= MouseSensitivity;
//
//		Yaw += xoffset;
//		Pitch += yoffset;
//
//		if (Pitch > 89.0f)
//			Pitch = 89.0f;
//		if (Pitch < -89.0f)
//			Pitch = -89.0f;
//		UpdateCameraVector();
//	}
//	void CalculateMouseScroll(float yoffset)
//	{
//		if (Fov >= 1.0f && Fov <= 45.0f)
//			Fov -= yoffset;
//		if (Fov <= 1.0f)
//			Fov = 1.0f;
//		if (Fov >= 45.0f)
//			Fov = 45.0f;
//	}
//	glm::mat4 GetProjectionMatrix()
//	{
//		return glm::perspective(glm::radians(Fov), AspectRatio, Near, Far);
//	}
//
//	glm::mat4 GetViewMatrix()
//	{
//		return CalculateLookAt();
//	}
//
//private:
//	void UpdateCameraVector()
//	{
//		glm::vec3 Target;
//		Target.x =  cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//		Target.y = sin(glm::radians(Pitch));
//		Target.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//		CameraTarget = glm::normalize(Target);
//		std::cout << "UpdateCameraVector CameraTarget.x=" << CameraTarget.x << " CameraTarget.y=" << CameraTarget.y << " CameraTarget.z=" << CameraTarget.z << std::endl;
//		CameraRight = glm::normalize(glm::cross(CameraTarget, WorldUp));// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//		CameraUp = glm::normalize(glm::cross(CameraRight, CameraTarget));
//	}
//
//	glm::mat4 CalculateLookAt()
//	{
//		return glm::lookAt(CameraPosition, CameraPosition + CameraTarget, CameraUp);
//	}
//};
#pragma once

#include "glm/mat4x4.hpp"
#include "GraphicsWindow.h"

#include <memory>

class Camera {

public:
	Camera(std::weak_ptr<GraphicsWindow> window);

	void setFOV(float FOV);
	void setFrustumRange(const glm::vec2& FrustumRanges);
	void updateProjectionMatrix();
	void updateViewMatrix();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	
	void update();

private:
	std::weak_ptr<GraphicsWindow> m_Window;

	float m_MoveSpeed = .3f;
	float m_MouseSensitivity = 0.1f;
	float m_FOV = 70.f;
	glm::vec2 m_FrustumRanges = glm::vec2(0.1f, 700.f);
	
	glm::vec3 m_Position = glm::vec3(0, 0, 0);
	glm::vec3 m_Forward = glm::vec3(0, 0, 1);
	glm::vec3 m_Up = glm::vec3(0, 1, 0);

	// Yaw - Pitch
	glm::vec2 m_Rotation;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

	glm::vec2 m_LastMousePosition = glm::vec2(0);

	void handleMouseInput();
	void handleKeyboardInput();
};
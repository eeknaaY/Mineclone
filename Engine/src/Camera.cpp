#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

Camera::Camera(std::shared_ptr<GraphicsWindow> window) {
	m_Window = window;
	setFrustumRange(m_FrustumRanges);
	setFOV(m_FOV);
}

void Camera::setFOV(float FOV) {
	if (FOV < 0) return;

	m_FOV = FOV;
	updateProjectionMatrix();
}

void Camera::setFrustumRange(glm::vec2 FrustumRanges) {
	if (FrustumRanges.x > FrustumRanges.y) return;

	m_FrustumRanges = FrustumRanges;
	updateProjectionMatrix();
}

void Camera::updateProjectionMatrix() {
	int w_Width = 800, w_Height = 800;
	glfwGetWindowSize(m_Window->getRawWindow(), &w_Width, &w_Height);

	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), (float)w_Width / (float)w_Height, m_FrustumRanges.x, m_FrustumRanges.y);
}

void Camera::updateViewMatrix() {
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, glm::vec3(0, 1, 0));
	std::cout << m_Position.x << " : " << m_Position.y << " : " << m_Position.z << " : " << "\n";
}

glm::mat4 Camera::getProjectionMatrix() {
	return m_ProjectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
	return m_ViewMatrix;
}

void Camera::update() {
	handleKeyboardInput();
	handleMouseInput();
	updateViewMatrix();
}

void Camera::handleMouseInput() {
	glm::vec2 mousePosition = m_Window->getMousePosition();
	glm::vec2 deltaMousePosition = glm::vec2(mousePosition.x - m_LastMousePosition.x, m_LastMousePosition.y - mousePosition.y);
	m_LastMousePosition = mousePosition;

	deltaMousePosition *= m_MouseSensitivity;

	m_Rotation += deltaMousePosition;
	if (m_Rotation.y > 89.f) m_Rotation.y = 89.f;
	if (m_Rotation.y < -89.f) m_Rotation.y = -89.f;

	glm::vec3 cameraFront;
	cameraFront.x = cos(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));
	cameraFront.y = sin(glm::radians(m_Rotation.y));
	cameraFront.z = sin(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));
	m_Forward = glm::normalize(cameraFront);
}

void Camera::handleKeyboardInput() {
	if (glfwGetKey(m_Window->getRawWindow(), GLFW_KEY_W) == GLFW_PRESS)
		m_Position += m_MoveSpeed * glm::normalize(glm::vec3(m_Forward.x * cos(m_Rotation.y * 3.14 / 180), 0, m_Forward.z * cos(m_Rotation.y * 3.14 / 180)));
	if (glfwGetKey(m_Window->getRawWindow(), GLFW_KEY_S) == GLFW_PRESS)
		m_Position -= m_MoveSpeed * glm::normalize(glm::vec3(m_Forward.x * cos(m_Rotation.y * 3.14 / 180), 0, m_Forward.z * cos(m_Rotation.y * 3.14 / 180)));
	if (glfwGetKey(m_Window->getRawWindow(), GLFW_KEY_A) == GLFW_PRESS)
		m_Position -= glm::normalize(glm::cross(m_Forward, m_Up)) * m_MoveSpeed;
	if (glfwGetKey(m_Window->getRawWindow(), GLFW_KEY_D) == GLFW_PRESS)
		m_Position += glm::normalize(glm::cross(m_Forward, m_Up)) * m_MoveSpeed;
	if (glfwGetKey(m_Window->getRawWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		m_Position += m_MoveSpeed * m_Up;
	if (glfwGetKey(m_Window->getRawWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_Position -= m_MoveSpeed * m_Up;
}

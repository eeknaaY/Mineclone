#include "GraphicsWindow.h"

#include "ActionHandler.h"

#include <iostream>


GraphicsWindow::GraphicsWindow() {
	// Failed to load glfw
	if (glfwInit() == 0) {
		std::cout << "Failed to load glfw" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->m_GLFWwindow = glfwCreateWindow(800, 800, "Mineclone", NULL, NULL);

	// Failed to initialize a window
	if (!m_GLFWwindow) {
		std::cout << "Failed to initialize glfw window" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(m_GLFWwindow);
	glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(m_GLFWwindow, this);
	glfwSetKeyCallback(m_GLFWwindow, GraphicsWindow::keyCallback);


	if (!gladLoadGL()) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

bool GraphicsWindow::isActive() {
	return !glfwWindowShouldClose(m_GLFWwindow);
}

void GraphicsWindow::destroy() {
	glfwSetWindowShouldClose(m_GLFWwindow, true);
}

void GraphicsWindow::swap() {
	glfwSwapBuffers(m_GLFWwindow);
}

void GraphicsWindow::pollEvents() {
	glfwPollEvents();
}

GLFWwindow* GraphicsWindow::getRawWindow() {
	return m_GLFWwindow;
}

glm::vec2 GraphicsWindow::getMousePosition() {
	GLdouble xPos, yPos;
	glfwGetCursorPos(m_GLFWwindow, &xPos, &yPos);

	return {xPos, yPos};
}

void GraphicsWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) Engine::ActionHandler::PerformAction(key);
}

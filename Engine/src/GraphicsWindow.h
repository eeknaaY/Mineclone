#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include "Keybind.h"

class GraphicsWindow {
	
public:
	GraphicsWindow();
	bool isActive();
	void swap();
	void pollEvents();
	void destroy();
	glm::vec2 getMousePosition();
	GLFWwindow* getRawWindow();

private:
	GLFWwindow* m_GLFWwindow;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
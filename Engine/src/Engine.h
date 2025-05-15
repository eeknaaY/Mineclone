#pragma once

#include "Camera.h"
#include "GraphicsWindow.h"
#include "Shaders/Shader.h"
#include "Keybind.h"
#include "SSBOMesh.h"

#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace Engine {
	extern std::shared_ptr<Camera> MainCamera;
	extern std::shared_ptr<GraphicsWindow> MainWindow;
	GraphicsWindow& Initialize();
	void Stop();
	void Update();
};


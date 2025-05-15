#pragma once

#include "Keybind.h"
#include <unordered_map>
#include <functional>
#include "GLFW/glfw3.h"

namespace Engine {

class Keybind {
public:
	enum Actions {
		STOP_ENGINE,
		BLANK
	};
	
	static void SetAction(Actions action, std::function<void()> function);
	static void PerformAction(int GLFW_KEY);
	static void SetKeybind(int GLFW_KEY, Actions action);
	static void SetStartUpKeybinds();

private:
	static std::unordered_map <int, Actions> m_KeyToActionMap;
	static std::unordered_map <Actions, std::function<void()>> m_ActionToFunctionMap;
};

}
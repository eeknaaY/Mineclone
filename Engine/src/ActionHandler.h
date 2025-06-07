#pragma once

#include <unordered_map>
#include <functional>
#include "GLFW/glfw3.h"

namespace Engine {

	class ActionHandler {
	public:
		static void SetAction(int action, std::function<void()> function);
		static void PerformAction(int GLFW_KEY);
		static void SetKeybind(int GLFW_KEY, int action);
		static void SetStartUpKeybinds();

	private:
		static std::unordered_map <int, int> m_KeyToActionMap;
		static std::unordered_map <int, std::function<void()>> m_ActionToFunctionMap;
	};

}
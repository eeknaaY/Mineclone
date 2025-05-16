#include "Keybind.h"

namespace Engine {

	std::unordered_map<int, int> Keybind::m_KeyToActionMap;
	std::unordered_map<int, std::function<void()>> Keybind::m_ActionToFunctionMap;

	void Keybind::SetKeybind(int GLFW_KEY, int action) {
		m_KeyToActionMap[GLFW_KEY] = action;
	}

	void Keybind::SetStartUpKeybinds() {
		// TODO: Implement?
	}

	void Keybind::PerformAction(int GLFW_KEY) {
		// Keybind has not been set.
		if (!m_KeyToActionMap.count(GLFW_KEY)) return;

		int action = m_KeyToActionMap[GLFW_KEY];
		// Function has not been set. (All functions should be set beforehand, just a precaution)
		if (!m_ActionToFunctionMap.count(action)) return;

		m_ActionToFunctionMap[action]();
	}

	void Keybind::SetAction(int action, std::function<void()> function) {

		m_ActionToFunctionMap[action] = function;
	}
}

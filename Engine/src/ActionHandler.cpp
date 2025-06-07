#include "ActionHandler.h"

namespace Engine {

	std::unordered_map<int, int> ActionHandler::m_KeyToActionMap;
	std::unordered_map<int, std::function<void()>> ActionHandler::m_ActionToFunctionMap;

	void ActionHandler::SetKeybind(int GLFW_KEY, int action) {
		m_KeyToActionMap[GLFW_KEY] = action;
	}

	void ActionHandler::SetStartUpKeybinds() {
		// TODO: Implement?
	}

	void ActionHandler::PerformAction(int GLFW_KEY) {
		// Keybind has not been set.
		if (!m_KeyToActionMap.count(GLFW_KEY)) return;

		int action = m_KeyToActionMap[GLFW_KEY];
		// Function has not been set. (All functions should be set beforehand, just a precaution)
		if (!m_ActionToFunctionMap.count(action)) return;

		m_ActionToFunctionMap[action]();
	}

	void ActionHandler::SetAction(int action, std::function<void()> function) {

		m_ActionToFunctionMap[action] = function;
	}
}

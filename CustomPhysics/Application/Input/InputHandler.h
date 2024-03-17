#pragma once

#ifndef InputHandler_H
#define InputHandler_H

#include <unordered_map>
#include "KeyCallbackEntry.h"
#include <unordered_set>
#include "MouseCallbackEntry.h"

namespace CustomApplication
{
	class InputHandler
	{
	private:
		static const int MAX_KEYS = 256;

		bool m_keyStatePrev[MAX_KEYS];
		bool m_keyState[MAX_KEYS];

		int m_mouseX = 0;
		int m_mouseY = 0;

		std::unordered_set<MouseMoveCallbackEntry> m_onMouseMoveCallback;
		std::unordered_map<uint32_t, std::unordered_set<KeyCallbackEntry>> m_onKeyPressCallbacks;
		std::unordered_map<uint32_t, std::unordered_set<KeyCallbackEntry>> m_onKeyReleaseCallbacks;
		std::unordered_map<uint32_t, std::unordered_set<KeyCallbackEntry>> m_onKeyHoldCallbacks;

	public:
		InputHandler() = default;

		bool Init();
		void PostInit();
		void Release();

		void HandleInput(double dt);

		void KeyPress(unsigned char key, int x, int y);
		void KeySpecial(int key, int x, int y);
		void KeyRelease(int key, int x, int y);

		void MouseMotion(int x, int y);
		void MouseEvent(int button, int state, int x, int y);

		void SubscribeOnMouseMoveEvent(MouseMoveCallbackEntry& callback);
		void SubscribeOnKeyPress(uint32_t key, KeyCallbackEntry& callback);
		void SubscribeOnKeyRelease(uint32_t key, KeyCallbackEntry& callback);
		void SubscribeOnKeyHold(uint32_t key, KeyCallbackEntry& callback);
	};
}

#endif
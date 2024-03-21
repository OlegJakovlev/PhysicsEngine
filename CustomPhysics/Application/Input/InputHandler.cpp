#include "InputHandler.h"
#include <stdio.h>

namespace CustomApplication
{
	bool InputHandler::Init()
	{
		return true;
	}

	void InputHandler::PostInit()
	{
	}

	void InputHandler::Release()
	{
	}

	void InputHandler::HandleInput(double dt)
	{
		for (int keyID = 0; keyID < MAX_KEYS; keyID++)
		{
			bool currentlyPressed = m_keyState[keyID];
			bool previouslyPressed = m_keyStatePrev[keyID];

			if (currentlyPressed && !previouslyPressed)
			{
				printf("KeyPressed: %c\n", keyID);

				auto events = m_onKeyPressCallbacks.find(keyID);
				if (events != m_onKeyPressCallbacks.end())
				{
					for (auto& entry : m_onKeyPressCallbacks[keyID])
					{
						entry.m_callback(dt);
					}
				}
			}

			if (currentlyPressed && previouslyPressed)
			{
				printf("KeyHold: %c\n", keyID);

				auto events = m_onKeyHoldCallbacks.find(keyID);
				if (events != m_onKeyHoldCallbacks.end())
				{
					for (auto& entry : m_onKeyHoldCallbacks.at(keyID))
					{
						entry.m_callback(dt);
					}
				}
			}

			if (!currentlyPressed && previouslyPressed)
			{
				printf("KeyReleased: %c\n", keyID);

				auto events = m_onKeyReleaseCallbacks.find(keyID);
				if (events != m_onKeyReleaseCallbacks.end())
				{
					for (auto& entry : m_onKeyReleaseCallbacks.at(keyID))
					{
						entry.m_callback(dt);
					}
				}
			}

			// Update prev states
			m_keyStatePrev[keyID] = m_keyState[keyID];
		}
	}

	void InputHandler::KeyPress(unsigned char key, int x, int y)
	{
		m_keyState[key] = true;
	}

	void InputHandler::KeySpecial(int key, int x, int y)
	{
		m_keyState[key] = true;
	}

	void InputHandler::KeyRelease(int key, int x, int y)
	{
		m_keyState[key] = false;
	}

	void InputHandler::MouseMotion(int x, int y)
	{
		int dx = m_mouseX - x;
		int dy = m_mouseY - y;

		for (auto& entry : m_onMouseMoveCallback)
		{
			entry.m_callback(dx, dy, x, y, 1 / 60.0f);
		}

		m_mouseX = x;
		m_mouseY = y;
	}

	void InputHandler::MouseEvent(int button, int state, int x, int y)
	{
		m_mouseX = x;
		m_mouseY = y;
	}

	void InputHandler::SubscribeOnMouseMoveEvent(MouseMoveCallbackEntry& callback)
	{
		m_onMouseMoveCallback.emplace(callback);
	}

	void InputHandler::SubscribeOnKeyPress(uint32_t key, KeyCallbackEntry& callback)
	{
		m_onKeyPressCallbacks[key].emplace(callback);
	}

	void InputHandler::SubscribeOnKeyRelease(uint32_t key, KeyCallbackEntry& callback)
	{
		m_onKeyReleaseCallbacks[key].emplace(callback);
	}

	void InputHandler::SubscribeOnKeyHold(uint32_t key, KeyCallbackEntry& callback)
	{
		m_onKeyHoldCallbacks[key].emplace(callback);
	}
}

#pragma once

#ifndef InputHandler_H
#define InputHandler_H

namespace CustomApplication
{
	class InputHandler
	{
	private:
		const int MAX_KEYS = 256;
		bool* m_keyState;
		int m_mouseX = 0;
		int m_mouseY = 0;

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
	};
}

#endif
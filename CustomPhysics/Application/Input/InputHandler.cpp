#include "InputHandler.h"
#include <stdio.h>

namespace CustomApplication
{
	bool InputHandler::Init()
	{
		m_keyState = new bool[256];
		return true;
	}

	void InputHandler::PostInit()
	{
	}

	void InputHandler::Release()
	{
	}

	void InputHandler::HandleInput()
	{
		for (int i = 0; i < MAX_KEYS; i++)
		{
			if (m_keyState[i])
			{
				// CameraInput(i);
				// ForceInput(i);
				// UserKeyHold(i);
			}
		}
	}

	void InputHandler::KeyPress(unsigned char key, int x, int y)
	{
		printf("KeyPressed: %c\n", key);
		switch (key)
		{
			case 'R':
				// scene->ExampleKeyPressHandler();
				break;

			default:
				break;
		}
	}

	void InputHandler::KeySpecial(int key, int x, int y)
	{
		/*
		  switch (key)
		  {
		  // display control
		  case GLUT_KEY_F5:
		  // hud on/off
		  hud_show = !hud_show;
		  break;

		  case GLUT_KEY_F6:
		  // shadows on/off
		  Renderer::ShowShadows(!Renderer::ShowShadows());
		  break;

		  case GLUT_KEY_F7:
		  // toggle render mode
		  ToggleRenderMode();
		  break;

		  case GLUT_KEY_F8:
		  // reset camera view
		  camera->Reset();
		  break;

		  // simulation control
		  case GLUT_KEY_F9:
		  // select next actor
		  scene->SelectNextActor();
		  break;

		  case GLUT_KEY_F10:
		  // toggle scene pause
		  scene->Pause(!scene->Pause());
		  break;

		  case GLUT_KEY_F12:
		  // resect scene
		  scene->Reset();
		  break;

		  default:
		  break;
		  }
		  */
	}

	void InputHandler::KeyRelease(int key, int x, int y)
	{
	}

	void InputHandler::MouseMotion(int x, int y)
	{
		int dx = m_mouseX - x;
		int dy = m_mouseY - y;

		//m_camera->Motion(dx, dy, delta_time);

		m_mouseX = x;
		m_mouseY = y;
	}

	void InputHandler::MouseEvent(int button, int state, int x, int y)
	{
		m_mouseX = x;
		m_mouseY = y;
	}
}

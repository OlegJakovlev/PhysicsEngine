#pragma once

#ifndef GlutApp_H
#define GlutApp_H

#include "Input/InputHandler.h"
#include "../PhysicsEngine/Engine/PhysicsEngine.h"
#include "Rendering/Renderer.h"
#include "SceneManagement/GameSceneManager.h"
#include <thread>
#include <mutex>

// I hate that original Glut does not have glutMainLoopEvent to use it just for rendering
// Because I'm not going to implement custom renderers for now, whole app will be based on this garbage
// There is AppLoop that needs to be adapted for generic application class
namespace CustomApplication
{
	class GlutApp
	{
	private:
		static GlutApp* s_instance;

		InputHandler* m_input;
		PhysicsEngine::PhysicsEngine* m_physicsEngine;
		Renderer* m_renderer;

		GameSceneManager* m_sceneManager;

		// Glut is running on main thread, so input and renderer would not be separated :(
		std::mutex m_physicsMutex;
		std::condition_variable m_cv;
		std::thread m_physicsThread;
		bool m_running;

		static void ReshapeCallback(int width, int height);
		static void IdleCallback();
		static void RenderSceneCallback();
		static void KeyPressCallback(unsigned char key, int x, int y);
		static void KeySpecial(int key, int x, int y);
		static void KeyRelease(unsigned char key, int x, int y);
		static void MotionCallback(int x, int y);
		static void MouseCallback(int button, int state, int x, int y);
		static void ExitCallback(void);

		void LinkCallbacks();

		void StartPhysics();

	public:
		static GlutApp* Get();
		static void Release();

		bool Init(const char* window_name, int width, int height);
		bool PostInit();
		void Start();
	};
}


#endif
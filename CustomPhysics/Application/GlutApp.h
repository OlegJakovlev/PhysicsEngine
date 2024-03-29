#pragma once

#ifndef GlutApp_H
#define GlutApp_H

#include "Input/InputHandler.h"
#include "../PhysicsEngine/Engine/PhysicsEngine.h"
#include "Rendering/Renderer.h"
#include "SceneManagement/GameSceneManager.h"
#include <thread>
#include "Database/ColorDatabase.h"

// I hate that original Glut does not have glutMainLoopEvent to use it just for rendering
// Because I'm not going to implement custom renderers for now, whole app will be based on this garbage
// There is AppLoop that needs to be adapted for generic application class
namespace CustomApplication
{
	class GlutApp
	{
	private:
		static GlutApp* s_instance;

		const float k_inputStep = 1.0 / 60.0;
		const float k_physicsStep = 1.0 / 60.0;
		const float k_renderStep = 1.0 / 240.0;

		const float k_maxAllowedDeltaTime = 1.0f;
		bool m_running;

		InputHandler* m_input;
		PhysicsEngine::PhysicsEngine* m_physicsEngine;
		Renderer* m_renderer;

		ColorDatabase* m_colorDatabase;
		GameSceneManager* m_sceneManager;

		std::thread m_physicsThread;
		std::chrono::steady_clock::time_point m_previousTime;
		double m_inputLag;
		double m_renderLag;

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

		const ColorDatabase const* GetColorDatabase() const;
		const InputHandler const* GetInputHandler() const;
		const Renderer const* GetRenderer() const;

		const float GetInputStep() const;
		const float GetRenderStep() const;
	};
}


#endif
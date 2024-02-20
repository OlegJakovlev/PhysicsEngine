#include "GlutApp.h"
#include <GL/glut.h>
#include "Rendering/RendererFactory.h"
#include "SceneManagement/GameScene.h"

namespace CustomApplication
{
	GlutApp* GlutApp::s_instance = nullptr;

	GlutApp* GlutApp::Get()
	{
		if (s_instance == nullptr)
		{
			s_instance = new GlutApp();
		}

		return s_instance;
	}

	void GlutApp::Release()
	{
		if (s_instance->m_physicsThread.joinable())
		{
			s_instance->m_physicsThread.join();
		}

		s_instance->m_input->Release();
		delete s_instance->m_input;

		s_instance->m_physicsEngine->Release();
		delete s_instance->m_physicsEngine;

		s_instance->m_renderer->Release();
		delete s_instance->m_renderer;

		delete s_instance;
		s_instance = nullptr;
	}

	void GlutApp::StartPhysics()
	{
		std::chrono::steady_clock::time_point prevTime = std::chrono::steady_clock::now();
		double timeStep = 1.0 / 60.0;

		while (m_running)
		{
			std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedTime = currentTime - prevTime;

			if (elapsedTime.count() >= timeStep)
			{
				// Update the previous time
				prevTime = currentTime;

				// Perform physics update
				m_physicsEngine->Update(timeStep);

				m_cv.notify_one();
			}
		}
	}

	void GlutApp::ReshapeCallback(int width, int height)
	{
		s_instance->m_renderer->Reshape(width, height);
	}

	void GlutApp::IdleCallback()
	{
		s_instance->m_renderer->Idle();
	}

	void GlutApp::RenderSceneCallback()
	{
		// Input
		s_instance->m_input->HandleInput();

		// Render
		std::unique_lock<std::mutex> lock(s_instance->m_physicsMutex);
		s_instance->m_cv.wait(lock);

		const std::unordered_set<GameScene*> activeScenesMap = *s_instance->m_sceneManager->GetActiveGameScenes();
		s_instance->m_renderer->Clear();

		for (auto it = activeScenesMap.cbegin(); it != activeScenesMap.cend(); it++)
		{
			s_instance->m_renderer->Render(*it);
		}

		s_instance->m_renderer->RenderHUD();
		s_instance->m_renderer->FinishRender();
	}

	void GlutApp::KeyPressCallback(unsigned char key, int x, int y)
	{
		s_instance->m_input->KeyPress(key, x, y);
	}

	void GlutApp::KeySpecial(int key, int x, int y)
	{
		s_instance->m_input->KeySpecial(key, x, y);
	}

	void GlutApp::KeyRelease(unsigned char key, int x, int y)
	{
		s_instance->m_input->KeyRelease(key, x, y);
	}

	void GlutApp::MotionCallback(int x, int y)
	{
		s_instance->m_input->MouseMotion(x, y);
	}

	void GlutApp::MouseCallback(int button, int state, int x, int y)
	{
		s_instance->m_input->MouseEvent(button, state, x, y);
	}

	void GlutApp::ExitCallback(void)
	{
	}

	bool GlutApp::Init(const char* name, int width, int height)
	{
		m_input = new InputHandler();
		if (!m_input->Init())
		{
			return false;
		}

		m_physicsEngine = new PhysicsEngine::PhysicsEngine();
		if (!m_physicsEngine->Init())
		{
			return false;
		}

		m_renderer = new Renderer();
		m_renderer->SetImpl(RendererFactory::CreateRenderer(RendererType::Glut));

		if (!m_renderer->Init(name, width, height))
		{
			return false;
		}

		m_sceneManager = new GameSceneManager();
		if (!m_sceneManager->Init(m_physicsEngine))
		{
			return false;
		}

		LinkCallbacks();
		return true;
	}

	bool GlutApp::PostInit()
	{
		m_input->PostInit();
		m_physicsEngine->PostInit();
		m_renderer->PostInit();

		return true;
	}

	void GlutApp::LinkCallbacks()
	{
		glutReshapeFunc(GlutApp::ReshapeCallback);
		glutIdleFunc(GlutApp::IdleCallback);

		// Render
		glutDisplayFunc(GlutApp::RenderSceneCallback);

		// Keyboard
		glutKeyboardFunc(GlutApp::KeyPressCallback);
		glutSpecialFunc(GlutApp::KeySpecial);
		glutKeyboardUpFunc(GlutApp::KeyRelease);

		// Mouse
		glutMouseFunc(GlutApp::MouseCallback);
		glutMotionFunc(GlutApp::MotionCallback);

		// Exit
		atexit(GlutApp::ExitCallback);

		// Init motion callback
		GlutApp::MotionCallback(0, 0);
	}

	void GlutApp::Start()
	{
		m_running = true;
		m_physicsThread = std::thread(&GlutApp::StartPhysics, this);
		glutMainLoop();
	}
}

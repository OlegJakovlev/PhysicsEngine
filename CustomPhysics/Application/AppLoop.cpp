#ifdef false

#include "AppLoop.h"
#include "../Engine/PhysicsEngine.h"
#include "Rendering/RendererFactory.h"

namespace CustomApplication
{
	bool AppLoop::Init(const UpdateFunction updateFunc, const SceneManager* sceneManager)
	{
		k_sceneManager = sceneManager;

#ifdef DEBUG_INPUT
		m_inputHandler = new InputHandler();
		m_inputHandler->Init();
#endif

#ifdef DEBUG_RENDERER
		m_renderer = new Renderer();
		m_renderer->SetImpl(RendererFactory::CreateRenderer(k_rendererType));
		m_renderer->Init("Olegs Jakovlevs", 800, 600);
		m_renderer->PostInit();
#endif

		m_fixedUpdate = updateFunc;

		m_timer = new Timer();
		return true;
	}

	AppLoop::~AppLoop()
	{
		delete m_timer;
		m_timer = nullptr;
	}

	// Adapted from previous work:
	// https://github.com/OlegJakovlev/SDL_Engine/blob/master/SDL_Engine/Engine/GameLoop/GameLoop.cpp
	void AppLoop::Run()
	{
#ifdef DEBUG_INPUT
		m_inputLag = 0;
#endif

		int physicsUpdates = 0;
		m_previousTime = m_timer->GetCurrentTime();

		while (true)
		{
			double currentTime = m_timer->GetCurrentTime();
			double frameTime = currentTime - m_previousTime;

			// Ensure we are not in breakpoint or having a performance drop
			if (frameTime > k_maxAllowedDeltaTime)
			{
				frameTime = k_maxAllowedDeltaTime;
			}

			m_previousTime = currentTime;
			physicsUpdates = 0;

#ifdef DEBUG_INPUT
			m_inputLag += frameTime;
			if (m_inputLag >= k_secondsPerInput)
			{
				m_inputHandler->HandleInput();
				m_inputLag -= k_secondsPerInput;
				currentTime += k_secondsPerInput;
			}
#endif

			m_updateLag += frameTime;
			if (m_updateLag >= k_secondsPerInput)
			{
				while (m_updateLag >= k_secondsPerUpdate && physicsUpdates < k_maxPhysicsUpdates)
				{
					m_fixedUpdate();
					m_updateLag -= k_secondsPerUpdate;
					currentTime += k_secondsPerUpdate;
				}
			}

			//Update();

#ifdef DEBUG_RENDERER
			const std::unordered_set<Scene*> activeScenesMap = *k_sceneManager->GetActiveScenes();

			for (auto it = activeScenesMap.cbegin(); it != activeScenesMap.cend(); it++)
			{
				const Scene* scene = *it;
				m_renderer->Render(scene->GetStaticActors(), scene->GetStaticActorCount());
				m_renderer->Render(scene->GetDynamicActors(), scene->GetDynamicActorCount());
			}

			m_renderer->FinishRender();
#endif
		}
	}
}

#endif
#pragma once

// This class meant to be a way to run custom app loop in physics engine, but later was extracted to be part of the application
// TODO: Refactor so it would be possible to use any renderer
#ifdef false
#define AppLoop_H

#include "../GlobalDefine.h"
#include "../Utility/Timer.h"
#include <functional>

#include "../Engine/SceneManager.h"
#include "Input/InputHandler.h"
#include "Rendering/Renderer.h"
#include "Rendering/RendererType.h"

namespace CustomApplication
{
	class AppLoop
	{
	private:
		typedef std::function<void()> UpdateFunction;

		static constexpr float k_secondsPerInput = 0.033333; // 30 fps
		static constexpr float k_secondsPerUpdate = 0.01666; // 60 fps
		static constexpr int k_maxPhysicsUpdates = 60;
		static constexpr float k_maxAllowedDeltaTime = k_secondsPerUpdate * 10;

		const SceneManager* k_sceneManager;

		double m_inputLag;
		InputHandler* m_inputHandler;

		static constexpr RendererType k_rendererType = RendererType::Glut;
		Renderer* m_renderer;

		// Variables
		double m_updateLag;
		double m_previousTime;
		Timer* m_timer;
		UpdateFunction m_fixedUpdate;

	public:
		AppLoop() = default;
		~AppLoop();

		bool Init(const UpdateFunction updateFunc, const SceneManager* sceneManager);
		void Run();
	};
}

#endif
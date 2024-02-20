#pragma once

// This class meant to be a way to run custom app loop, but currently not being in used
#ifdef AppLoop_H
#define AppLoop_H

#include "../GlobalDefine.h"
#include "../Utility/Timer.h"
#include <functional>

#include "../Engine/SceneManager.h"

#ifdef DEBUG_INPUT
#include "Input/InputHandler.h"
#endif

#ifdef DEBUG_RENDERER
#include "Rendering/Renderer.h"
#include "Rendering/RendererType.h"
#endif

// Class purely designed for testing purposes
class AppLoop
{
private:
	typedef std::function<void()> UpdateFunction;

	static constexpr float k_secondsPerInput = 0.033333; // 30 fps
	static constexpr float k_secondsPerUpdate = 0.01666; // 60 fps
	static constexpr int k_maxPhysicsUpdates = 60;
	static constexpr float k_maxAllowedDeltaTime = k_secondsPerUpdate * 10;

	const SceneManager* k_sceneManager;

#ifdef DEBUG_INPUT
	double m_inputLag;
	InputHandler* m_inputHandler;
#endif

#ifdef DEBUG_RENDERER
	static constexpr RendererType k_rendererType = RendererType::Glut;
	Renderer* m_renderer;
#endif

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

#endif
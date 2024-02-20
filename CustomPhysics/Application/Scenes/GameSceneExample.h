#pragma once

#include "../SceneManagement/GameScene.h"

namespace CustomApplication
{
	class GameSceneExample : public GameScene
	{
	public:
		void Init(const PhysicsEngine::PhysicsEngine* physicsEngine) override;
	};
}


#pragma once

#ifndef PlayGameScene_H
#define PlayGameScene_H

#include "../SceneManagement/GameScene.h"

namespace CustomApplication
{
	class PlayGameScene : public GameScene
	{
	public:
		void Init(const PhysicsEngine::PhysicsEngine* physicsEngine) override;
		void Update(float dt) override;
		void FixedUpdate(float dt) override;
		void FixedPhysicsUpdate(float dt) override;
	};
}

#endif
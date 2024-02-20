#pragma once

#include "../SceneManagement/GameScene.h"

class GameSceneExample : public GameScene
{
public:
	void Init(const PhysicsEngine* physicsEngine) override;
};


#pragma once

#ifndef GameSceneManager_H
#define GameSceneManager_H

#include <unordered_set>
#include <unordered_map>
#include "GameScene.h"

class GameSceneManager
{
private:
	std::unordered_map<uint32_t, GameScene*> m_gameScenes;
	std::unordered_set<GameScene*> m_activeGameScenes;

public:
	bool Init(const PhysicsEngine* physicsEngine);
	const std::unordered_set<GameScene*>* GetActiveGameScenes() const;
};

#endif

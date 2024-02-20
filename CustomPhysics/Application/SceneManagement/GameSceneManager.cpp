#include "GameSceneManager.h"
#include "../Scenes/GameSceneExample.h"

bool GameSceneManager::Init(const PhysicsEngine* physicsEngine)
{
	// TODO: Move to configurator
	GameScene* m_gameScene = new GameSceneExample();
	m_gameScene->Init(physicsEngine);

	m_gameScenes.emplace(0, m_gameScene);
	m_activeGameScenes.emplace(m_gameScene);

	return true;
}

const std::unordered_set<GameScene*>* GameSceneManager::GetActiveGameScenes() const
{
	return &m_activeGameScenes;
}

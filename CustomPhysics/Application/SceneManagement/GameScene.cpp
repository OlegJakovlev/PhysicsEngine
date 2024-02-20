#include "GameScene.h"

namespace CustomApplication
{
	void GameScene::AddGameActorInternal(StaticGameObject* staticGameObject)
	{
		m_staticGameObjects[m_staticGameObjectCount] = staticGameObject;
		m_staticGameObjectCount++;
	}

	void GameScene::AddGameActorInternal(DynamicGameObject* dynamicGameObject)
	{
		m_dynamicGameObjects[m_dynamicGameObjectCount] = dynamicGameObject;
		m_dynamicGameObjectCount++;
	}

	void GameScene::AddGameActor(GameObject* gameObject)
	{
		GameObject::Type underlayingType = gameObject->GetType();

		if (underlayingType == GameObject::Type::Static)
		{
			StaticGameObject* staticGameObject = static_cast<StaticGameObject*>(gameObject);
			AddGameActorInternal(staticGameObject);
			return;
		}

		if (underlayingType == GameObject::Type::Dynamic)
		{
			DynamicGameObject* dynamicGameObject = static_cast<DynamicGameObject*>(gameObject);
			AddGameActorInternal(dynamicGameObject);
			return;
		}

		std::printf("Unknown underlaying GameObject type!");
		return;
	}

	void GameScene::Init(const PhysicsEngine::PhysicsEngine* physicsEngine)
	{
		m_staticGameObjectCount = 0;
		m_dynamicGameObjectCount = 0;

		m_staticGameObjects = new GameObject * [k_maxStaticGameObjects];
		m_dynamicGameObjects = new GameObject * [k_maxDynamicGameObjects];

		gameObjectFactory = new GameObjectFactory();
		gameObjectFactory->Init(const_cast<PhysicsEngine::ActorFactory*>(physicsEngine->GetActorFactory()));
	}

	const void* GameScene::GetPhysicsScene() const
	{
		return m_physicsScene;
	}

	const GameObject** GameScene::GetStaticActors() const
	{
		return const_cast<const GameObject**>(m_staticGameObjects);
	}

	const uint32_t GameScene::GetStaticActorCount() const
	{
		return m_staticGameObjectCount;
	}

	const GameObject** GameScene::GetDynamicActors() const
	{
		return const_cast<const GameObject**>(m_dynamicGameObjects);
	}

	const uint32_t GameScene::GetDynamicActorCount() const
	{
		return m_dynamicGameObjectCount;
	}

}

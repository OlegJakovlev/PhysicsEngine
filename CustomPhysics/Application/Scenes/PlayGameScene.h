#pragma once

#ifndef PlayGameScene_H
#define PlayGameScene_H

#include "../SceneManagement/GameScene.h"

namespace CustomApplication
{
	class PlayGameScene : public GameScene
	{
	private:
		void GameFieldMarking(const PhysicsEngine::ShapeCreator* shapeCreator, const PhysicsEngine::GeometryFactory* geoFactory, float minHeightOffset);
		void GameFieldOuterWalls(const PhysicsEngine::ShapeCreator* shapeCreator, const PhysicsEngine::GeometryFactory* geoFactory, float minHeightOffset);

	public:
		void Init(const PhysicsEngine::PhysicsEngine* physicsEngine) override;
		void Update(float dt) override;
		void FixedUpdate(float dt) override;
		void FixedPhysicsUpdate(float dt) override;
	};
}

#endif
#pragma once

#ifndef PlayGameScene_H
#define PlayGameScene_H

#include "../SceneManagement/GameScene.h"

namespace CustomApplication
{
	class PlayGameScene : public GameScene
	{
	private:
		void GameFieldMarking(const PhysicsEngine::ShapeFactory* shapeCreator, const PhysicsEngine::GeometryFactory* geoFactory, float minHeightOffset, float innerFieldWidth, float innerFieldDepth);
		void GameFieldOuterWalls(const PhysicsEngine::ShapeFactory* shapeCreator, const PhysicsEngine::GeometryFactory* geoFactory, float minHeightOffset, float innerFieldWidth, float innerFieldDepth);
		void GoalObjects(const PhysicsEngine::GeometryFactory* geoFactory, const PhysicsEngine::ShapeFactory* shapeCreator, float innerFieldWidth, float innerFieldDepth);
		void CornerPolesAndFlags(const PhysicsEngine::GeometryFactory* geoFactory, const PhysicsEngine::ShapeFactory* shapeCreator, float innerFieldWidth, float innerFieldDepth);

	public:
		void Init(const PhysicsEngine::PhysicsEngine* physicsEngine) override;
		void Update(float dt) override;
		void FixedUpdate(float dt) override;
		void FixedPhysicsUpdate(float dt) override;
	};
}

#endif
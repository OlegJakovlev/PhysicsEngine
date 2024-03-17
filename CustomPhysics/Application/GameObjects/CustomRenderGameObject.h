#pragma once

#ifndef CustomRenderGameObject_H
#define CustomRenderGameObject_H

#include "GameObject.h"
#include "../DataTypes/AppVoidCallbackEntry.h"

namespace CustomApplication
{
	class CustomRenderGameObject : public GameObject
	{
		friend class GameObjectFactory;

	private:
		const AppVoidCallbackEntry* m_renderCallback;

	protected:
		CustomRenderGameObject() = delete;
		CustomRenderGameObject(const uint64_t id);

	public:
		void SetCustomRenderCallback(const AppVoidCallbackEntry& renderCallback);
		void CustomRender();
	};
}

#endif
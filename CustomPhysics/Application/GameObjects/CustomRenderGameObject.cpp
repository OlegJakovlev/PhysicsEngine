#include "CustomRenderGameObject.h"

namespace CustomApplication
{
	CustomRenderGameObject::CustomRenderGameObject(const uint64_t id) : GameObject(id, GameObject::Type::Custom)
	{
	}

	void CustomRenderGameObject::SetCustomRenderCallback(const AppVoidCallbackEntry& renderCallback)
	{
		m_renderCallback = &renderCallback;
	}

	void CustomRenderGameObject::CustomRender()
	{
		m_renderCallback->m_callback();
	}
}

#pragma once

#ifndef RendererType_H
#define RendererType_H

namespace CustomApplication
{
	enum class RendererType
	{
		Glut,
		Vulkan,
		DirectX11,
		SDL,
	};
}

#endif
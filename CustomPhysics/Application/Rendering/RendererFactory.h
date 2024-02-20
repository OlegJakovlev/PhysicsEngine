#pragma once

#ifndef RendererFactory_H
#define RendererFactory_H

#include "Renderers/IRender.h"
#include "RendererType.h"

namespace CustomApplication
{
	class RendererFactory
	{
	public:
		static IRender* CreateRenderer(RendererType type);
	};
}


#endif
#include "RendererFactory.h"
#include "Renderers/GlutRenderer.h"

namespace CustomApplication
{
	IRender* RendererFactory::CreateRenderer(RendererType type)
	{
		switch (type)
		{
			case (RendererType::Glut):
				return new GlutRenderer();

			default:
				return nullptr;
		}
	}

}

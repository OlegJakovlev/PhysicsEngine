#include "RendererFactory.h"
#include "Renderers/GlutRenderer.h"

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

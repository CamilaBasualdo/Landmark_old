#pragma once
#include <ECS/Component/Component.h>
#include <vulkan/vulkan.hpp>

namespace Landmark
{
	namespace Render
	{
		class Renderer;
	}
}


class TestRenderComponent : public Landmark::ECS::Component<TestRenderComponent>
{
	friend Landmark::Render::Renderer;
public:
	void Render()
	{
		
		//LOGGER.Log("Rendering");
	}

};


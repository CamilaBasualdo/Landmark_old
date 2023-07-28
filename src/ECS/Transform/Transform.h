#pragma once
#include <General_Utils.h>
#include <iostream>
#include <ECS/Component/Component.h>
namespace Landmark::ECS
{
	class Transform :Component<Transform>
	{
		int pos;
		vec3  Position;
		quat Rotation;
		vec3 Scale;


		Transform(): Component<Transform>()
		{
			
			std::cout << "Testing Transform Component";
		}
	};

	


}

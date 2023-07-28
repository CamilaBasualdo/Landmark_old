#pragma once
#include <vulkan/vulkan.hpp>
#include <Logger.h>
#include <map>

namespace Landmark::Vk
{
	 class GraphicsPipeline
	 {
		 static inline Logger LOGGER = Logger("GraphicsPipeline");
	 public:
		 enum ShaderStage
		 {
			 VERTEX = VK_SHADER_STAGE_VERTEX_BIT,
			 GEOMETRY = VK_SHADER_STAGE_GEOMETRY_BIT,
			 FRAGMENT = VK_SHADER_STAGE_FRAGMENT_BIT
		 };
		 enum TopologyType
		 {
			 TOPOLOGY_TRIANGLE = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			 TOPOLOGY_LINE = VK_PRIMITIVE_TOPOLOGY_LINE_LIST
		 };
		 enum CullingMode
		 {
			 CULL_BACK = VK_CULL_MODE_BACK_BIT,
			 CULL_FRONT = VK_CULL_MODE_FRONT_BIT,
			 CULL_FRONT_AND_BACK = VK_CULL_MODE_FRONT_AND_BACK,
			 CULL_NONE = VK_CULL_MODE_NONE
		 };
		enum PolygonMode
		{
			POLYGON_FILL = VK_POLYGON_MODE_FILL,
			POLYGON_LINE = VK_POLYGON_MODE_LINE,
			POLYGON_POINT = VK_POLYGON_MODE_POINT
		};
		enum DepthCompareOp
		{
			DEPTH_OP_NEVER = VK_COMPARE_OP_NEVER,
			DEPTH_OP_LESS = VK_COMPARE_OP_LESS,
			DEPTH_OP_EQUAL = VK_COMPARE_OP_EQUAL,
			DEPTH_OP_LESS_OR_EQUAL = VK_COMPARE_OP_LESS_OR_EQUAL,
			DEPTH_OP_GREATER = VK_COMPARE_OP_GREATER,
			DEPTH_OP_NOT_EQUAL = VK_COMPARE_OP_NOT_EQUAL,
			DEPTH_OP_GREATER_OR_EQUAL = VK_COMPARE_OP_GREATER_OR_EQUAL,
			DEPTH_OP_ALWAYS = VK_COMPARE_OP_ALWAYS
		};
	 private:
		 VkPipelineLayout PipelineLayout;
		 VkPipeline graphicsPipeline;

	 	std::map<ShaderStage,VkShaderModule> ShaderModules;
		 std::vector<VkDynamicState> DynamicStates = { VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_SCISSOR };
		 TopologyType TopologyMode =TOPOLOGY_TRIANGLE;

		 uint32_t ViewportCount = 1;
		 uint32_t ScissorCount = 1;

		 CullingMode cullingMode = CULL_NONE;
		 float LineWidth = 1.0f;
		 PolygonMode polygonMode = POLYGON_FILL;
		 bool DiscardEnable = false;
		 bool DepthClampEnable = true;

		 bool DepthTestingState = true;
		 bool DepthWritingState = true;
		 DepthCompareOp DepthComparingOperation = DEPTH_OP_LESS_OR_EQUAL;

	 public:
		 GraphicsPipeline& AttachModule(ShaderStage stage, const std::vector<char>& src);
		 GraphicsPipeline& SetDynamicStates(std::initializer_list<VkDynamicState> States);
		 GraphicsPipeline& DefineInputAssembly(TopologyType _TopologyType);
		 GraphicsPipeline& SetViewportState(uint32_t _ViewportCount, uint32_t _ScissorCount);

		 
		//Rasterizer Shit
		 GraphicsPipeline& Raz_SetCullMode(CullingMode _mode);
		 GraphicsPipeline& Raz_SetLineWidth(float _LineWidth);
		 GraphicsPipeline& Raz_PolygonMode(PolygonMode _PolygonMode);
		 GraphicsPipeline& Raz_DiscardEnable(bool state);
		 GraphicsPipeline& Raz_DepthClampEnable(bool state);


		 GraphicsPipeline& SetDepthState(bool _DepthTestingEnable, bool _DepthWritingEnable, DepthCompareOp _CompareOp);


		 void Build();
	 };

}

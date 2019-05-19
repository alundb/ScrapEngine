#include "VulkanGraphicsPipeline.h"

#include "../Base/Vertex.h"
#include "../Base/StaticTypes.h"

ScrapEngine::Render::VulkanGraphicsPipeline::VulkanGraphicsPipeline(const char* vertex_shader,
                                                                    const char* fragment_shader,
                                                                    vk::Extent2D* swap_chain_extent,
                                                                    vk::DescriptorSetLayout* descriptor_set_layout,
                                                                    vk::SampleCountFlagBits msaa_samples,
                                                                    bool is_skybox)
{
	shader_manager_ref_ = new ShaderManager();

	auto vert_shader_code = ShaderManager::read_file(vertex_shader);
	auto frag_shader_code = ShaderManager::read_file(fragment_shader);

	vk::ShaderModule vert_shader_module = shader_manager_ref_->create_shader_module(vert_shader_code);
	vk::ShaderModule frag_shader_module = shader_manager_ref_->create_shader_module(frag_shader_code);

	vk::PipelineShaderStageCreateInfo vert_shader_stage_info(
		vk::PipelineShaderStageCreateFlags(),
		vk::ShaderStageFlagBits::eVertex,
		vert_shader_module,
		"main"
	);

	vk::PipelineShaderStageCreateInfo frag_shader_stage_info(
		vk::PipelineShaderStageCreateFlags(),
		vk::ShaderStageFlagBits::eFragment,
		frag_shader_module,
		"main"
	);

	vk::PipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

	auto attribute_descriptions = Vertex::get_attribute_descriptions();
	auto binding_description = Vertex::get_binding_description();

	vk::PipelineVertexInputStateCreateInfo vertex_input_info(
		vk::PipelineVertexInputStateCreateFlags(),
		1,
		&binding_description,
		static_cast<uint32_t>(attribute_descriptions.size()),
		attribute_descriptions.data()
	);

	vk::PipelineInputAssemblyStateCreateInfo input_assembly(
		vk::PipelineInputAssemblyStateCreateFlags(),
		vk::PrimitiveTopology::eTriangleList,
		false
	);

	vk::Viewport viewport(
		0,
		0,
		static_cast<float>(swap_chain_extent->width),
		static_cast<float>(swap_chain_extent->height),
		0.0f,
		1.0f);

	vk::Rect2D scissor(
		vk::Offset2D(),
		*swap_chain_extent
	);

	vk::PipelineViewportStateCreateInfo viewport_state(
		vk::PipelineViewportStateCreateFlags(),
		1,
		&viewport,
		1,
		&scissor
	);

	vk::PipelineRasterizationStateCreateInfo rasterizer(
		vk::PipelineRasterizationStateCreateFlags(),
		false,
		false,
		vk::PolygonMode::eFill,
		vk::CullModeFlagBits::eBack,
		vk::FrontFace::eCounterClockwise
	);
	rasterizer.setLineWidth(1.0f);

	vk::PipelineMultisampleStateCreateInfo multisampling(
		vk::PipelineMultisampleStateCreateFlags(),
		msaa_samples
	);

	vk::PipelineDepthStencilStateCreateInfo depth_stencil(
		vk::PipelineDepthStencilStateCreateFlags(),
		true,
		true,
		vk::CompareOp::eLessOrEqual
	);

	vk::PipelineColorBlendAttachmentState color_blend_attachment;
	color_blend_attachment.setColorWriteMask(
		vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::
		ColorComponentFlagBits::eA);

	vk::PipelineColorBlendStateCreateInfo color_blending(
		vk::PipelineColorBlendStateCreateFlags(),
		false,
		vk::LogicOp::eCopy,
		1,
		&color_blend_attachment
	);

	vk::PipelineLayoutCreateInfo pipeline_layout_info(
		vk::PipelineLayoutCreateFlags(),
		1,
		&(*descriptor_set_layout)
	);

	if (is_skybox)
	{
		rasterizer.setCullMode(vk::CullModeFlagBits::eFront);
		depth_stencil.setDepthWriteEnable(false);
		depth_stencil.setDepthTestEnable(false);
	}

	if (VulkanDevice::static_logic_device_ref->createPipelineLayout(&pipeline_layout_info, nullptr, &pipeline_layout_)
		!= vk::Result::eSuccess)
	{
		throw std::runtime_error("VulkanGraphicsPipeline: Failed to create pipeline layout!");
	}

	vk::GraphicsPipelineCreateInfo pipeline_info(
		vk::PipelineCreateFlags(),
		2,
		shader_stages,
		&vertex_input_info,
		&input_assembly,
		nullptr,
		&viewport_state,
		&rasterizer,
		&multisampling,
		&depth_stencil,
		&color_blending,
		nullptr,
		pipeline_layout_,
		*VulkanRenderPass::static_render_pass_ref,
		0
	);

	if (VulkanDevice::static_logic_device_ref->createGraphicsPipelines(nullptr, 1, &pipeline_info, nullptr,
	                                                                   &graphics_pipeline_) != vk::Result::eSuccess)
	{
		throw std::runtime_error("VulkanGraphicsPipeline: Failed to create graphics pipeline!");
	}

	VulkanDevice::static_logic_device_ref->destroyShaderModule(frag_shader_module);
	VulkanDevice::static_logic_device_ref->destroyShaderModule(vert_shader_module);
	delete shader_manager_ref_;
}


ScrapEngine::Render::VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
	VulkanDevice::static_logic_device_ref->destroyPipeline(graphics_pipeline_);
	VulkanDevice::static_logic_device_ref->destroyPipelineLayout(pipeline_layout_);
}

vk::Pipeline* ScrapEngine::Render::VulkanGraphicsPipeline::get_graphics_pipeline()
{
	return &graphics_pipeline_;
}

vk::PipelineLayout* ScrapEngine::Render::VulkanGraphicsPipeline::get_pipeline_layout()
{
	return &pipeline_layout_;
}

#pragma once

#include "GfxAPI/vulkan/AnniVulkan.h"


namespace Anni2
{
	enum class RsrcType
	{
		Unknown,
		Texture,
		Attachment,
		ModelTextures,
		Buffer,
		IndexBuffer,
		VertexBuffer,
		ModelBuffer,
		AddressRefedBuffer
	};

	enum class RsrcAccessTypeRG
	{
		Unknown,
		Write,
		Read,
		ReadWrite,
	};

	struct DescriptorSetInfo
	{
		uint32_t set{ 0 };
		uint32_t binding{ 0 };
		uint32_t array_element{ 0 };
	};

	struct DescriptorInfo
	{
		DescriptorSetInfo slot_info;
		vk::DescriptorType descriptor_type;
		vk::ShaderStageFlags desc_shader_stages_flags;
	};

}



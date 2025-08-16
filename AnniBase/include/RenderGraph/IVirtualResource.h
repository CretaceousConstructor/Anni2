#pragma once

#include "GfxComponents/QueueManager.h"
#include "Resource/Resource.h"
#include <map>

namespace Anni2::RenderGraph
{
	class GraphicsPassNode;
}

namespace Anni2::RenderGraph
{
	enum class VRsrcType
	{
		Imported,
		Established
	};

	class IVirtualResource
	{
	public:
		explicit IVirtualResource(std::string name_, VRsrcType rsrc_type_);
		IVirtualResource() = delete;

		IVirtualResource(const IVirtualResource&) = default;
		IVirtualResource& operator=(const IVirtualResource&) = default;

		IVirtualResource(IVirtualResource&&) = default;
		IVirtualResource& operator=(IVirtualResource&&) = default;

		virtual ~IVirtualResource() = default;


	public:
		std::string                                  name;
		VRsrcType                                    rsrc_type;

		std::map<size_t, std::vector<GraphicsPassNode*>>                   level_to_passes_attached_to;
		std::map<size_t, std::vector<std::vector<GraphicsPassNode*>>>      level_to_passes_attached_to_partitioned;
		std::vector<std::pair<GraphicsPassNode*, Anni2::RsrcAccessTypeRG>> passes_access_this_rsrc;
	};
}






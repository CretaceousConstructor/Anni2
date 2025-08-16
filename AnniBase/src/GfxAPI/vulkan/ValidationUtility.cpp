#include "GfxAPI/vulkan/ValidationUtility.h"

namespace Anni2::ValidationUtility
{

	const std::array<const char*, 1> required_validation_instance_layers = 
	{
		   "VK_LAYER_KHRONOS_validation",
	};

	bool CheckIfRequiredInstanceLayersSupported()
	{
		//之前的版本有device（某个gpu） layer和instance（整个程序）layer的区别，不过已经不再这么区分，全部都用instance layer
		//遍历所有可用的LAYER
		//*************************************************************
		const std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties().value;

		/*  获取所有可以用的layer
			VK_LAYER_NV_optimus
			VK_LAYER_NV_nsight
			VK_LAYER_RENDERDOC_Capture
			VK_LAYER_NV_nomad_release_public_2021_1_1
			VK_LAYER_NV_GPU_Trace_release_public_2021_1_1
			VK_LAYER_VALVE_steam_overlay
			VK_LAYER_VALVE_steam_fossilize
			VK_LAYER_LUNARG_api_dump
			VK_LAYER_LUNARG_device_simulation
			VK_LAYER_LUNARG_gfxreconstruct
			VK_LAYER_KHRONOS_synchronization2
			VK_LAYER_KHRONOS_validation
			VK_LAYER_LUNARG_monitor
			VK_LAYER_LUNARG_screenshot
		*/
		//*************************************************************

		//遍历需要用到的INSTANCE LAYER，看看需要的用到的在不在可用列表中。只要有一个不能用就返回false
		//*************************************************************
		std::vector<const char*> required_instance_layer;

		if ( VALIDATION_LAYERS_ENABLED )
		{
			required_instance_layer.insert(required_instance_layer.end(), required_validation_instance_layers.begin(), required_validation_instance_layers.end());
		}

		for ( auto layer_name : required_instance_layer )
		{
			bool layer_found = false;
			for ( const auto& layer_properties : available_layers )
			{
				if ( strcmp(layer_name, layer_properties.layerName) == 0 )
				{
					layer_found = true;
					break;
				}
			}
			if ( !layer_found )
			{
				return false;
			}
		}
		return true;
		//*************************************************************
	}



	void SetupDebugMessenger(vk::Instance instance, vk::UniqueDebugUtilsMessengerEXT& debug_messenger)
	{
		if ( !ValidationUtility::VALIDATION_LAYERS_ENABLED )
		{
			return;
		}

		vk::DebugUtilsMessengerCreateInfoEXT create_info{};
		ValidationUtility::PopulateDebugMessengerCreateInfo(create_info);
		debug_messenger = instance.createDebugUtilsMessengerEXTUnique(create_info).value;
	}


	void PopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& create_info)
	{
		create_info.messageSeverity = vk::FlagTraits<vk::DebugUtilsMessageSeverityFlagBitsEXT>::allFlags;
		create_info.messageType = vk::FlagTraits<vk::DebugUtilsMessageTypeFlagBitsEXT>::allFlags;
		create_info.pfnUserCallback = &DebugCallback;
	}




  //typedef VULKAN_HPP_NAMESPACE::Bool32( VKAPI_PTR * PFN_DebugUtilsMessengerCallbackEXT )(
  //  VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
  //  VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT              messageTypes,
  //  const VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
  //  void *                                                           pUserData );

	vk::Bool32 VKAPI_PTR DebugCallback(
		vk::DebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
		vk::DebugUtilsMessageTypeFlagsEXT              messageTypes,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		//std::ofstream file1;
		//file1.open("D:/CS/ComputerGraphics/vulkan/WindowsProject1/log.txt", std::ios::app);

		const std::string message(pCallbackData->pMessage);
		std::string       debugMessage("DEBUG-PRINTF ]");


		if ( messageSeverity & (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError) )
		{
			std::cout << "[VALIDATION LAYER]: " << "\n"
				<< "********************************************************************************" << "\n";
			constexpr auto size_one_line = 140;
			for ( size_t i = 0; i < message.length(); i += size_one_line )
			{
				std::cout << message.substr(i, size_one_line) << "\n";
			}

			PrintDebugUtilsObjectNameInfo(pCallbackData->objectCount, pCallbackData->pObjects);
			std::cout << "********************************************************************************" << "\n";
		}
		return VK_FALSE;
	}


	VKAPI_ATTR void PrintDebugUtilsObjectNameInfo(const uint32_t num_of_obj, const vk::DebugUtilsObjectNameInfoEXT* const object_names)
	{
		for ( size_t i = 0; i < num_of_obj; i++ )
		{
			if ( (object_names + i)->pObjectName )
			{
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
				std::cout << (object_names + i)->pObjectName << std::endl;
				std::cout << "--------------------------------------------------------------------------------" << std::endl;
			}
		}
	}




}        // namespace Anni::ValidationUtility

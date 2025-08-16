#pragma once
#include "GfxAPI/vulkan/ExtensionUtility.h"
#include "GfxAPI/vulkan/ValidationUtility.h"

namespace Anni2
{

	class InstanceWrapper
	{
	private:
		static const std::string app_name;
		static const std::string engine_name;

	private:
		void CreateVulkanInstance();
		//TODO:InitWindowBackendSystem和ShutDownWindowBackendSystem两个函数在整个程序中应该只调用一次
		static void InitWindowBackendSystem();
		static void ShutDownWindowBackendSystem();
		//GLFW的错误回调函数
		static void GlfwErrorCallback(int error, const char* description);

	private:
		//C++ bindings
		//vk::raii::Context context;
		vk::UniqueInstance instance;
		vk::UniqueDebugUtilsMessengerEXT debug_messenger;

	public:
		vk::Instance GetInstance();
		explicit InstanceWrapper();
		~InstanceWrapper();

		InstanceWrapper(const InstanceWrapper&) = delete;
		InstanceWrapper& operator=(const InstanceWrapper&) = delete;

		InstanceWrapper(InstanceWrapper&&) = delete;
		InstanceWrapper& operator=(InstanceWrapper&&) = delete;
	};
}        // namespace Anni


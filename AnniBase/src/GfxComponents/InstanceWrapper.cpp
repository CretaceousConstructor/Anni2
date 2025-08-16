#include "GfxComponents/InstanceWrapper.h"

namespace Anni2
{
	const std::string InstanceWrapper::app_name{ "DivineRapier" };
	const std::string InstanceWrapper::engine_name{ "Annihilation" };

	InstanceWrapper::InstanceWrapper()
		//context()
	{
		InitWindowBackendSystem();
		CreateVulkanInstance();
		ValidationUtility::SetupDebugMessenger(*instance, debug_messenger);
	}

	InstanceWrapper::~InstanceWrapper()
	{
		ShutDownWindowBackendSystem();
	}

	vk::Instance InstanceWrapper::GetInstance()
	{
		return *instance;
	}

	void InstanceWrapper::InitWindowBackendSystem()
	{
		glfwSetErrorCallback(GlfwErrorCallback);//设置glfw错误回调函数。
		glfwInit();                             //glfw初始化。
	}

	void InstanceWrapper::ShutDownWindowBackendSystem()
	{
		glfwTerminate();
	}


	void InstanceWrapper::CreateVulkanInstance()
	{
		//检查 实例(instance) 会用到的 全局层instance layer(现在已经不区分全局层和设备层了，全都是全局层)
		ASSERT_WITH_MSG(ValidationUtility::CheckIfRequiredInstanceLayersSupported(), "some instance layers required, but not available!");

		// initialize the vk::ApplicationInfo structure
		const vk::ApplicationInfo applicationInfo(app_name.c_str(), 1, engine_name.c_str(), 2, ANNI_VK_API_VERSION);

		// initialize the vk::InstanceCreateInfo填写实例创建信息
		vk::InstanceCreateInfo instance_create_info({}, &applicationInfo);

		//生成vulkan实例的步骤
			//1.如果由我们自己处理validation层面输出的信息，则需要打开一些extensions
			//2.获得glfw会用到的实例扩展 并且打开！
			//之后的vkcreateinstance会用这里获得的值

		//获得 实例instance 会用到的 全局扩展(instance extensions。这里没有检查设备扩展(device extension)，之后才会检查)
		const auto extensions = ExtensionUtility::GetNeededGlobalInstanceExtensions(ValidationUtility::VALIDATION_LAYERS_ENABLED);
		instance_create_info.setPEnabledExtensionNames(extensions);
		//*********************************************************************

		vk::ValidationFeaturesEXT validation_feature_ext;
		validation_feature_ext.setEnabledValidationFeatures(ValidationUtility::enabled_validation_features);
		validation_feature_ext.setDisabledValidationFeatures(ValidationUtility::disabled_validation_features);

		vk::DebugUtilsMessengerCreateInfoEXT debug_util_messenger_create_info{};
		std::vector<const char*> RequiredInstanceLayer;
		//验证层需要的instance layer
		if (ValidationUtility::VALIDATION_LAYERS_ENABLED)
		{
			//获得 实例validation 会用到的 层(layer)，并且打开
			RequiredInstanceLayer.insert(RequiredInstanceLayer.end(), ValidationUtility::required_validation_instance_layers.begin(), ValidationUtility::required_validation_instance_layers.end());

			//下面的代码是为了捕获CreateDebugUtilsMessengerEXT和DestroyDebugUtilsMessengerEXT输出的validation信息
			ValidationUtility::PopulateDebugMessengerCreateInfo(debug_util_messenger_create_info);
			validation_feature_ext.pNext = &debug_util_messenger_create_info;                      //这样赋值pNext就可以输出CreateDebugUtilsMessengerEXT和DestroyDebugUtilsMessengerEXT输出的validation信息
			instance_create_info.pNext = &validation_feature_ext;
		}
		instance_create_info.setPEnabledLayerNames(RequiredInstanceLayer);
		instance = vk::createInstanceUnique(instance_create_info).value;
		VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());
	}

	void InstanceWrapper::GlfwErrorCallback(int error, const char* description)
	{
		fprintf(stderr, "GLFW ERROR %d: %s\n", error, description);
	}
}


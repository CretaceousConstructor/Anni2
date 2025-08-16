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
		glfwSetErrorCallback(GlfwErrorCallback);//����glfw����ص�������
		glfwInit();                             //glfw��ʼ����
	}

	void InstanceWrapper::ShutDownWindowBackendSystem()
	{
		glfwTerminate();
	}


	void InstanceWrapper::CreateVulkanInstance()
	{
		//��� ʵ��(instance) ���õ��� ȫ�ֲ�instance layer(�����Ѿ�������ȫ�ֲ���豸���ˣ�ȫ����ȫ�ֲ�)
		ASSERT_WITH_MSG(ValidationUtility::CheckIfRequiredInstanceLayersSupported(), "some instance layers required, but not available!");

		// initialize the vk::ApplicationInfo structure
		const vk::ApplicationInfo applicationInfo(app_name.c_str(), 1, engine_name.c_str(), 2, ANNI_VK_API_VERSION);

		// initialize the vk::InstanceCreateInfo��дʵ��������Ϣ
		vk::InstanceCreateInfo instance_create_info({}, &applicationInfo);

		//����vulkanʵ���Ĳ���
			//1.����������Լ�����validation�����������Ϣ������Ҫ��һЩextensions
			//2.���glfw���õ���ʵ����չ ���Ҵ򿪣�
			//֮���vkcreateinstance���������õ�ֵ

		//��� ʵ��instance ���õ��� ȫ����չ(instance extensions������û�м���豸��չ(device extension)��֮��Ż���)
		const auto extensions = ExtensionUtility::GetNeededGlobalInstanceExtensions(ValidationUtility::VALIDATION_LAYERS_ENABLED);
		instance_create_info.setPEnabledExtensionNames(extensions);
		//*********************************************************************

		vk::ValidationFeaturesEXT validation_feature_ext;
		validation_feature_ext.setEnabledValidationFeatures(ValidationUtility::enabled_validation_features);
		validation_feature_ext.setDisabledValidationFeatures(ValidationUtility::disabled_validation_features);

		vk::DebugUtilsMessengerCreateInfoEXT debug_util_messenger_create_info{};
		std::vector<const char*> RequiredInstanceLayer;
		//��֤����Ҫ��instance layer
		if (ValidationUtility::VALIDATION_LAYERS_ENABLED)
		{
			//��� ʵ��validation ���õ��� ��(layer)�����Ҵ�
			RequiredInstanceLayer.insert(RequiredInstanceLayer.end(), ValidationUtility::required_validation_instance_layers.begin(), ValidationUtility::required_validation_instance_layers.end());

			//����Ĵ�����Ϊ�˲���CreateDebugUtilsMessengerEXT��DestroyDebugUtilsMessengerEXT�����validation��Ϣ
			ValidationUtility::PopulateDebugMessengerCreateInfo(debug_util_messenger_create_info);
			validation_feature_ext.pNext = &debug_util_messenger_create_info;                      //������ֵpNext�Ϳ������CreateDebugUtilsMessengerEXT��DestroyDebugUtilsMessengerEXT�����validation��Ϣ
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


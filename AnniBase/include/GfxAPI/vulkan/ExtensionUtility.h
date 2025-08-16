#pragma once

#include "AnniVulkan.h"
#include "WindowsSubSystem/glfw/WindowsSubsystem.h"
#include <string>
#include <vector>

namespace Anni2::ExtensionUtility
{
	//global�������˼ָ������������������ȷ��˵��ĳ��instance��������������ĳ��device��GPU��
	std::vector<const char *>        GetNeededGlobalInstanceExtensions(bool ValidationLayerEnabled = false);
	const std::vector<const char *> &GetRequiredExtensionsForAGoodDevice();

};        // namespace Anni::ExtensionUtility

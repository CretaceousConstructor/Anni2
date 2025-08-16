#pragma once

#include "AnniVulkan.h"
#include "WindowsSubSystem/glfw/WindowsSubsystem.h"
#include <string>
#include <vector>

namespace Anni2::ExtensionUtility
{
	//global这里的意思指的是适用于整个程序（确切说是某个instance），而不仅仅是某个device（GPU）
	std::vector<const char *>        GetNeededGlobalInstanceExtensions(bool ValidationLayerEnabled = false);
	const std::vector<const char *> &GetRequiredExtensionsForAGoodDevice();

};        // namespace Anni::ExtensionUtility

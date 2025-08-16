#pragma once

#include "GfxComponents/InstanceWrapper.h"
#include "WindowsSubSystem/glfw/WindowsSubsystem.h"

#include <memory>
#include <string>

namespace Anni2
{
	class WindowsSys
	{
	private:
		InstanceWrapper& instance;
		GLFWwindow* raw_window_ptr{ nullptr };
		vk::UniqueSurfaceKHR surface_refac;

	public:
		const uint32_t    WIDTH;
		const uint32_t    HEIGHT;
		const std::string window_name;

	public:
		//800 600
		WindowsSys(InstanceWrapper& instance_, uint32_t w = 1280, uint32_t h = 740, std::string winName = "AnniRenderer");

	public:
		[[nodiscard]] vk::SurfaceKHR      GetSurface() const;
		[[nodiscard]] const GLFWwindow* GetWindowPtr() const;

	private:
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

	private:
		void CreateSurface();

	public:
		~WindowsSys();

		WindowsSys(const WindowsSys&) = delete;
		WindowsSys& operator=(const WindowsSys&) = delete;

		WindowsSys(WindowsSys&&) = delete;
		WindowsSys& operator=(WindowsSys&&) = delete;
	};
}        // namespace Anni

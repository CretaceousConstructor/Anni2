#include "WindowsSubSystem/WindowsSys.h"

namespace Anni2
{
	WindowsSys::WindowsSys(InstanceWrapper& instance_, uint32_t w, uint32_t h, std::string winName) :
		WIDTH(w), HEIGHT(h), window_name(std::move(winName)), instance(instance_)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);        //glfw窗口初始化,第一个参数是要设置的项目，第二个则是要设置的值，这里我们仅仅创建窗口，所以不用opengl的api
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);          //暂时禁止resize，之后会做处理

		raw_window_ptr = glfwCreateWindow(WIDTH, HEIGHT, window_name.c_str(), nullptr, nullptr);        //创建窗口

		glfwSetWindowUserPointer(raw_window_ptr, nullptr);                                //第二个参数之后可以用glfwGetWindowUserPointer函数取出但是目前就没有用过
		glfwSetFramebufferSizeCallback(raw_window_ptr, FrameBufferResizeCallback);        //framebufferResizeCallback是当用户调整窗口大小时调用的函数。

		CreateSurface();
	}

	WindowsSys::~WindowsSys()
	{
		//vkDestroySurfaceKHR(instance.GetInstance(), surface, nullptr);
		glfwDestroyWindow(raw_window_ptr);
	}

	void WindowsSys::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		//TODO：handle resize
	}

	vk::SurfaceKHR WindowsSys::GetSurface() const
	{
		return *surface_refac;
	}


	const GLFWwindow* WindowsSys::GetWindowPtr() const
	{
		return raw_window_ptr;        //return by value（会复制出去一个右值）
	}

	void WindowsSys::CreateSurface()
	{

		VkSurfaceKHR  surface;
		//创建windows的表面，这个函数应当在检查物理设备之前调用，会对物理设备的选择产生影响
		VK_CHECK_RESULT(glfwCreateWindowSurface(instance.GetInstance(), raw_window_ptr, nullptr, &surface));

		surface_refac = vk::UniqueSurfaceKHR(surface);

	}
	


}        // namespace Anni

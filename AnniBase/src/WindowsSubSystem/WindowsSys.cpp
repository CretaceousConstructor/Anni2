#include "WindowsSubSystem/WindowsSys.h"

namespace Anni2
{
	WindowsSys::WindowsSys(InstanceWrapper& instance_, uint32_t w, uint32_t h, std::string winName) :
		WIDTH(w), HEIGHT(h), window_name(std::move(winName)), instance(instance_)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);        //glfw���ڳ�ʼ��,��һ��������Ҫ���õ���Ŀ���ڶ�������Ҫ���õ�ֵ���������ǽ����������ڣ����Բ���opengl��api
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);          //��ʱ��ֹresize��֮���������

		raw_window_ptr = glfwCreateWindow(WIDTH, HEIGHT, window_name.c_str(), nullptr, nullptr);        //��������

		glfwSetWindowUserPointer(raw_window_ptr, nullptr);                                //�ڶ�������֮�������glfwGetWindowUserPointer����ȡ������Ŀǰ��û���ù�
		glfwSetFramebufferSizeCallback(raw_window_ptr, FrameBufferResizeCallback);        //framebufferResizeCallback�ǵ��û��������ڴ�Сʱ���õĺ�����

		CreateSurface();
	}

	WindowsSys::~WindowsSys()
	{
		//vkDestroySurfaceKHR(instance.GetInstance(), surface, nullptr);
		glfwDestroyWindow(raw_window_ptr);
	}

	void WindowsSys::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		//TODO��handle resize
	}

	vk::SurfaceKHR WindowsSys::GetSurface() const
	{
		return *surface_refac;
	}


	const GLFWwindow* WindowsSys::GetWindowPtr() const
	{
		return raw_window_ptr;        //return by value���Ḵ�Ƴ�ȥһ����ֵ��
	}

	void WindowsSys::CreateSurface()
	{

		VkSurfaceKHR  surface;
		//����windows�ı��棬�������Ӧ���ڼ�������豸֮ǰ���ã���������豸��ѡ�����Ӱ��
		VK_CHECK_RESULT(glfwCreateWindowSurface(instance.GetInstance(), raw_window_ptr, nullptr, &surface));

		surface_refac = vk::UniqueSurfaceKHR(surface);

	}
	


}        // namespace Anni

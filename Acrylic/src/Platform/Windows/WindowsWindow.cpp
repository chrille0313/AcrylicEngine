#include "acpch.h"
#include "WindowsWindow.h"

#include "Acrylic/Events/ApplicationEvent.h"
#include "Acrylic/Events/MouseEvent.h"
#include "Acrylic/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace Acrylic {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		AC_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		AC_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			// TODO: glfTerminate on system shutdown
			int success = glfwInit();
			AC_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}


		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
								  {
									  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
									  data.Width = width;
									  data.Height = height;

									  WindowResizeEvent e(width, height);
									  data.EventCallback(e);
								  });

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
								   {
									   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
									   WindowCloseEvent e;
									   data.EventCallback(e);
								   });

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
						   {
							   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							   switch (action) {
								   case GLFW_PRESS:
								   {
									   KeyPressedEvent e(key, 0);
									   data.EventCallback(e);
									   break;
								   }

								   case GLFW_RELEASE:
								   {
									   KeyReleasedEvent e(key);
									   data.EventCallback(e);
									   break;
								   }

								   case GLFW_REPEAT:
								   {
									   KeyPressedEvent e(key, 1);
									   data.EventCallback(e);
									   break;
								   }
							   }
						   });

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
							{
								WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
								KeyTypedEvent e(keycode);
								data.EventCallback(e);
							});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes)
								   {
									   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

									   switch (action) {
										   case GLFW_PRESS:
										   {
											   MouseButtonPressedEvent e(button);
											   data.EventCallback(e);
											   break;
										   }

										   case GLFW_RELEASE:
										   {
											   MouseButtonReleasedEvent e(button);
											   data.EventCallback(e);
											   break;
										   }
									   }
								   });

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
								 {
									 WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

									 MouseMovedEvent e((float)xPos, (float)yPos);
									 data.EventCallback(e);
								 });

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
							  {
								  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								  MouseScrolledEvent e((float)xOffset, (float)yOffset);
								  data.EventCallback(e);
							  });
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVsync() const
	{
		return m_Data.VSync;
	}
}
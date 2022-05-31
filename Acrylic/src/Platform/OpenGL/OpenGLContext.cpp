#include "acpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Acrylic {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		AC_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		AC_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AC_CORE_ASSERT(status, "Failed to initialize Glad!");
		AC_CORE_INFO("\n-------------------- OpenGL Info --------------------\n"
					 "Vendor: {0} \n"
					 "Renderer : {1} \n"
					 "Version : {2} \n"
					 "-----------------------------------------------------\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));

		AC_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Acrylic requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		AC_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}
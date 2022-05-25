#include "acpch.h"
#include "Acrylic/Core/Input.h"

#ifdef AC_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Acrylic {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
		#ifdef AC_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
		#else
		AC_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
		#endif
	}
}
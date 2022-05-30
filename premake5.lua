workspace "Acrylic"
	architecture "x86_64"
	startproject "Acrylic-Glass"

	configurations {
		"Debug",
		"Release",
		"Dist",
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Acrylic/vendor/GLFW/include"
IncludeDir["Glad"] = "Acrylic/vendor/Glad/include"
IncludeDir["ImGui"] = "Acrylic/vendor/imgui"
IncludeDir["glm"] = "Acrylic/vendor/glm"
IncludeDir["stb_image"] = "Acrylic/vendor/stb_image"

group "Dependencies"
	include "Acrylic/vendor/GLFW"
	include "Acrylic/vendor/Glad"
	include "Acrylic/vendor/imgui"

group ""

project "Acrylic"
	location "Acrylic"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "acpch.h"
	pchsource "Acrylic/src/acpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
		}

	filter "configurations:Debug"
		defines "AC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AC_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"Acrylic/vendor/spdlog/include",
		"Acrylic/src",
		"Acrylic/vendor",
		"%{IncludeDir.glm}",
	}

	links {
		"Acrylic"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "AC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AC_DIST"
		runtime "Release"
		optimize "on"


project "Acrylic-Glass"
	location "Acrylic-Glass"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"Acrylic/vendor/spdlog/include",
		"Acrylic/src",
		"Acrylic/vendor",
		"%{IncludeDir.glm}",
	}

	links {
		"Acrylic"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "AC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AC_DIST"
		runtime "Release"
		optimize "on"
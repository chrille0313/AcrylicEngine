project "Acrylic-Glass"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{wks.location}/Acrylic/vendor/spdlog/include",
		"%{wks.location}/Acrylic/src",
		"%{wks.location}/Acrylic/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
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

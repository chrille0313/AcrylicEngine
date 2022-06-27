include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Acrylic"
	architecture "x86_64"
	startproject "Acrylic-Glass"

	configurations {
		"Debug",
		"Release",
		"Dist",
	}

	solution_items {
		".editorconfig"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Acrylic/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Acrylic/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Acrylic/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Acrylic/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Acrylic/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Acrylic/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Acrylic/vendor/yaml-cpp/include"

group "Dependencies"
	include "vendor/premake"
	include "Acrylic/vendor/GLFW"
	include "Acrylic/vendor/Glad"
	include "Acrylic/vendor/imgui"
	--include "Acrylic/vendor/entt"
	include "Acrylic/vendor/yaml-cpp"

group ""

include "Acrylic"
include "Sandbox"
include "Acrylic-Glass"
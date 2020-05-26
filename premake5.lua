workspace "LIP"
    architecture "x86_64"

    configurations { 
        "Release", 
        "Debug"
    }

    targetdir "bin/%{cfg.buildcfg}/%{prj.name}"
    objdir "obj/%{cfg.buildcfg}/%{prj.name}"

    startproject "LIP_CLI"

    filter "system:Windows"
        systemversion "latest"

    filter "Release"
        defines { "NDEBUG", "RELEASE" }
        optimize "On"

    filter "Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "toolset:gcc"    -- GCC v8.x
        links "stdc++fs" 
    filter "toolset:clang"  -- clang
    filter {}

    -- LIP_Core project cotains logic and algorithms for LIP coding.
    project "LIP_Core"
        location "LIP_Core"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        targetname "lip_core"

        files {
            "%{prj.name}/src/**.hpp",
            "%{prj.name}/src/**.cpp"
        }

        filter "Release"
            optimize "Speed"

    -- Simple Command-Line Interface for LIP_Core. 
    -- There should be no critical logic here.
    project "LIP_CLI"
        location "LIP_CLI"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        targetname "lip"

        files {
            "%{prj.name}/src/**.hpp",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/vendor/CLI/CLI11.hpp"
        }

        includedirs {
            "LIP_Core/src",
            "%{prj.name}/vendor"
        }

        links {
            "LIP_Core"
        }


    group "Tests"

        -- Tests project for LIP_Core. It produces executable with
        -- embeded CLI from Catch2. Run this program with no arguments
        -- to simply run all the tests.
        project "LIP_Core_Tests"
            location "LIP_Core_Tests"
            kind "ConsoleApp"
            language "C++"
            cppdialect "C++17"
            targetname "lip_core_tests"

            files {
                "%{prj.name}/src/**.hpp",
                "%{prj.name}/src/**.cpp",
                "%{prj.name}/vendor/catch2/catch.hpp"
            }

            includedirs {
                "LIP_Core/src",
                "%{prj.name}/vendor"
            }

            links {
                "LIP_Core"
            }

    group ""


-- Additional actions for maintaining project files.
newaction {
   trigger     = "clean",
   description = "Clean project from binaries and obj files",
   execute = function ()
      print("Removing bin/...")
	  os.rmdir("./bin")
	  print("Removing obj/...")
	  os.rmdir("./obj")
	  print("Done")
   end
}

newaction {
	trigger     = "reset",
	description = "Removes all files ignored by git",
	execute = function()
		print("Removing generated files...")
		os.execute("git clean -Xdf")
		print("Done")
	end
}

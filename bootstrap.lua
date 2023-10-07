workspace "Chip8"
    location "./"
    language "C"
    architecture "x86"
    configurations { "Debug", "Development"}

    startproject "Emulator"
    characterset "ASCII"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"

    filter "configurations:Development"
        defines { "DEVELOPMENT" }
        optimize "On"

    filter { }

project "Emulator"
    location ""
    kind "ConsoleApp"
    targetdir "bin/"
    objdir ("intermediate/%{cfg.longname}")

    files {
                "source/**"
            ,   "extern/SDL2/include/"
          }
        
    includedirs { 
                      "source/"
                  ,   "extern/SDL2/include/"
                }
                
    links { "SDL2", "SDL2main" }
    
    libdirs { 
        "extern/SDL2/libs/"
    }
    workspace "Chip8"
    location "./"
    language "C++"
    cppdialect "C++20"
    architecture "x86"
    configurations { "Debug", "Development"}

    startproject "Emulator"
    characterset "ASCII"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"

    filter "configurations:Development"
        defines { "DEVELOPMENT" }
        optimize "On"

    filter { }

project "Emulator"
    location ""
    kind "ConsoleApp"
    targetdir "bin/"
    objdir ("intermediate/%{cfg.longname}")

    files {
                "source/**"
            ,   "extern/SDL2/include/"
          }
        
    includedirs { 
                      "source/"
                  ,   "extern/SDL2/include/"
                }
                
    links { "SDL2", "SDL2main" }
    
    libdirs { 
        "extern/SDL2/libs/"
    }

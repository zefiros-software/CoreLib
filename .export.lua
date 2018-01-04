
project "CoreLib"

    kind "StaticLib"       

    files "core/src/**.cpp"    

    zpm.export(function()
        includedirs "core/include/"
        cppdialect "C++14"
        pic "On" 

        defines {
            "PROGRAM_NAME=\"".. zpm.setting( "name" ) .. "\"",
            "PROGRAM_COMPANY=\"".. zpm.setting( "company" ) .. "\"",
            "PROGRAM_COPYRIGHT=\"".. zpm.setting( "copyright" ) .. "\""
        }    
    
        zpm.uses {
            "Zefiros-Software/MathLib",
            "Zefiros-Software/Boost",
            "Zefiros-Software/Date",
            "Zefiros-Software/Fmt"
        }

        filter "system:not macosx"
            linkgroups "On"

        filter "system:linux"            
            links {
                "pthread",
                "dl"
            }

        filter {}
    end)
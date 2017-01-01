
project "CoreLib"

    files "core/src/**.cpp"   
    kind "StaticLib"        

    zpm.export(function()
        includedirs "core/include/"
        flags "C++14"
        pic "On" 

        defines {
            "PROGRAM_NAME=\""..zpm.setting( "PROGRAM_NAME" ) .. "\"",
            "PROGRAM_COMPANY=\""..zpm.setting( "PROGRAM_COMPANY" ) .. "\"",
            "PROGRAM_COPYRIGHT=\""..zpm.setting( "PROGRAM_COPYRIGHT" ) .. "\""
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
    
    zpm.uses {
        "Zefiros-Software/DocoptCpp",
        "Zefiros-Software/SerLib",
        "Zefiros-Software/MathLib",
        "Zefiros-Software/Boost",
        "Zefiros-Software/Fmt"
    }
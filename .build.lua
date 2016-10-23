
project "CoreLib"

    files "core/src/**.cpp"   
    kind "StaticLib"        

    zpm.export [[
        includedirs "core/include/"
        flags "C++14"
        pic "On"     

        filter "system:not macosx"
            linkgroups "On"

        filter "system:linux"            
            links {
                "pthread",
                "dl"
            }

        filter {}
    ]]
    
    zpm.uses {
        "Zefiros-Software/DocoptCpp",
        "Zefiros-Software/SerLib",
        "Zefiros-Software/MathLib",
        "Zefiros-Software/Boost",
        "Zefiros-Software/Fmt",
        "Zefiros-Software/libsimdpp"
    }
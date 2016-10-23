
project "CoreLib"

    files "core/src/**.cpp"   
    kind "StaticLib"        

    zpm.export [[
        includedirs "core/include/"
        flags "C++14"
    
        filter "system:not macosx"
            linkgroups "On"

        filter "system:linux"            
            links "dl"

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
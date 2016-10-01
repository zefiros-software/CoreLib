
project "CoreLib"

    files "core/src/**.cpp"   
    kind "StaticLib"        

    zpm.export [[
        includedirs "core/include/"
        flags "C++11"
    ]]
    
    zpm.uses {
        "Zefiros-Software/GoogleTest",
        "Zefiros-Software/DocoptCpp",
        "Zefiros-Software/BSPLib",
        "Zefiros-Software/SerLib",
        "Zefiros-Software/MathLib",
        "Zefiros-Software/Boost",
        "Zefiros-Software/Fmt",
        "Zefiros-Software/libsimdpp",
        "Zefiros-Software/Slacking"
    }
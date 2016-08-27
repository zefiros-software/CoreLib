
project "CoreLib"

    files "core/src/**.cpp"   
    kind "StaticLib"        

    zpm.export [[
        includedirs "core/include/"
        flags "C++11"
    ]]
    
    zpm.uses {
        "Zefiros-Software/GoogleTest",
        "Zefiros-Software/BSPLib",
        "Zefiros-Software/SerLib",
        "Zefiros-Software/BenchLib",
        "Zefiros-Software/Boost",
        "Zefiros-Software/Fmt"
    }
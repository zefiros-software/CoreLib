local zefiros = require( "Zefiros-Software/Zefiros-Defaults", "@head" )

workspace "CoreLib"

	zefiros.setDefaults( "core" )
     
    project "core"
        zpm.uses {
            "Zefiros-Software/GoogleTest",
            "Zefiros-Software/BSPLib",
            "Zefiros-Software/SerLib",
            "Zefiros-Software/BenchLib",
            "Zefiros-Software/Boost",
            "Zefiros-Software/Fmt"
        }
     
    project "core-test"
        zpm.uses {
            "Zefiros-Software/GoogleTest",
            "Zefiros-Software/BSPLib",
            "Zefiros-Software/SerLib",
            "Zefiros-Software/BenchLib",
            "Zefiros-Software/Boost",
            "Zefiros-Software/Fmt"
        }
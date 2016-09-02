local zefiros = require( "Zefiros-Software/Zefiros-Defaults", "@head" )

workspace "CoreLib"

	zefiros.setDefaults( "core" )
     
    project "core"
        zpm.uses {
            "Zefiros-Software/GoogleBenchmark",
            "Zefiros-Software/GoogleTest",
            "Zefiros-Software/DocoptCpp",
            "Zefiros-Software/BSPLib",
            "Zefiros-Software/SerLib",
            "Zefiros-Software/MathLib",
            "Zefiros-Software/BenchLib",
            "Zefiros-Software/Boost",
            "Zefiros-Software/Fmt",
            "Zefiros-Software/libsimdpp"
        }
     
    project "core-test"
        zpm.uses {
            "Zefiros-Software/GoogleBenchmark",
            "Zefiros-Software/GoogleTest",
            "Zefiros-Software/DocoptCpp",
            "Zefiros-Software/BSPLib",
            "Zefiros-Software/SerLib",
            "Zefiros-Software/MathLib",
            "Zefiros-Software/BenchLib",
            "Zefiros-Software/Boost",
            "Zefiros-Software/Fmt",
            "Zefiros-Software/libsimdpp"
        }
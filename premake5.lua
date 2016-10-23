local zefiros = require( "Zefiros-Software/Zefiros-Defaults", "@head" )


function useCore()
    zpm.uses {
        "Zefiros-Software/DocoptCpp",
        "Zefiros-Software/SerLib",
        "Zefiros-Software/MathLib",
        "Zefiros-Software/Boost",
        "Zefiros-Software/Fmt",
        "Zefiros-Software/libsimdpp"
    }

end

workspace "CoreLib"   
        
    pic "On"     

    filter "system:linux"            
        links "dl"

    filter {}

	zefiros.setDefaults( "core" )
     
    project "core"
        useCore()
        
    project "core-test"
        useCore()       
        
        zpm.uses {
            "Zefiros-Software/GoogleTest",
        }
            
        links {
            "core", 
            "core-plugin-test4"
        }
                    
        includedirs {
            "plugin/test4/include/"
            }	
        
    
    group( "Plugins/" )
        project "core-plugin-main"

            kind "ConsoleApp"   
            
            links "core"

            useCore()        
                    
            includedirs {
                "core/include/",
                "plugin/"
                }				
                
            files { 
                "plugin/*.hpp",
                "plugin/*.h",
                "plugin/*.cpp"
                }

        project "core-plugin-test"        
            targetname "Test"
            kind "SharedLib"   
            targetsuffix "" 
            
            links "core"
            useCore()    
        
            zpm.uses {
                "Zefiros-Software/GoogleTest",
            }
                    
            includedirs {
                "core/include/"
                }				
                
            files { 
                "plugin/include/**.hpp",
                "plugin/include/**.h",
                "plugin/test/src/**.cpp"
                }
        
            filter "platforms:x86"
                targetdir "bin/x86/plugins/"
            
            filter "platforms:x86_64"
                targetdir "bin/x86_64/plugins/"

            filter {}

    group( "Plugins/Test2" )
        project "core-plugin-test2"        
            targetname "Test2"
            kind "SharedLib"   
            targetsuffix ""  
            
            links {
                "core", 
                "core-plugin-test2-lib"
            }

            useCore()
                    
            includedirs {
                "core/include/",
                "plugin/test2/include/"
                }	

            files { 
                "plugin/test2/src/plugin.cpp"
                }
        
            filter "platforms:x86"
                targetdir "bin/x86/plugins/"
            
            filter "platforms:x86_64"
                targetdir "bin/x86_64/plugins/"

            filter {}

        project "core-plugin-test2-lib"        
            targetname "Test2"
            kind "StaticLib"   
            
            links "core"

            useCore()
                    
            includedirs {
                "core/include/",
                "plugin/test2/include/"
                }						     			
                
            files { 
                "plugin/include/**.hpp",
                "plugin/include/**.h",
                "plugin/test2/src/test2.cpp"
                }

    group( "Plugins/Test3" )
        project "core-plugin-test3"        
            targetname "Test3"
            kind "SharedLib"   
            targetsuffix "" 
            
            links {
                "core",
                "core-plugin-test3-lib",
                "core-plugin-test2-lib"
            }

            useCore()
                    
            includedirs {
                "core/include/",
                "plugin/test3/include/",
                "plugin/test2/include/"
                }				
                
            files { 
                "plugin/test3/src/plugin.cpp"
                }
        
            filter "platforms:x86"
                targetdir "bin/x86/plugins/"
            
            filter "platforms:x86_64"
                targetdir "bin/x86_64/plugins/"

            filter {}

        project "core-plugin-test3-lib"        
            targetname "Test3"
            kind "StaticLib" 
            
            links {
                "core", 
                "core-plugin-test2-lib"
            }

            useCore()
                    
            includedirs {
                "core/include/",
                "plugin/test3/include/",
                "plugin/test2/include/"
                }				
                
            files { 
                "plugin/include/**.hpp",
                "plugin/include/**.h",
                "plugin/test3/src/test3.cpp"
                }
        

    group( "Plugins/Test4" )
        project "core-plugin-test4"        
            targetname "Test4"
            kind "StaticLib"
            
            links "core"

            useCore()
                    
            includedirs {
                "core/include/",
                "plugin/test4/include/"
                }						     			
                
            files { 
                "plugin/include/**.hpp",
                "plugin/include/**.h",
                "plugin/test4/src/**.cpp"
                }
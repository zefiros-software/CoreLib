-- [[
-- @cond ___LICENSE___
--
-- Copyright (c) 2016-2018 Zefiros Software.
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.
--
-- @endcond
-- ]]
function useCore()
    zpm.uses {
        "Zefiros-Software/MathLib",
        "Zefiros-Software/Boost",
        "Zefiros-Software/Date",
        "Zefiros-Software/Fmt"
    }

end

workspace "CoreLib"   
        
    cppdialect "C++14"
    pic "On"     

    filter "system:linux"            
        links {
            "pthread",
            "dl"
        }

    filter {}

	zefiros.setDefaults("core")
 
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
        
    
    group "Plugins/"
        project "core-plugin-test"    
            targetname "Test"
            kind "SharedLib"   
            targetsuffix "" 
            
            links "core"
            useCore()    
        
            zpm.uses {
                "Zefiros-Software/GoogleTest"
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

    group "Plugins/Test2"
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

    group "Plugins/Test3"
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
        

    group "Plugins/Test4"
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
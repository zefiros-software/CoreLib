--[[ @cond ___LICENSE___
-- Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
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
--]]

local zefiros = require( "Zefiros-Software/Zefiros-Defaults", "@head" )


workspace "CoreLib-ZPM"

	zefiros.setTestZPMDefaults( "core" )
    
	project "core-zpm-test"
        zpm.uses {
            "Zefiros-Software/CoreLib",
            "Zefiros-Software/GoogleTest"
        }            
            
        links {
            "core-plugin-test4"
        }
                    
        includedirs {
            "../plugin/test4/include/"
            }	
        
    group "Plugins/Test4"
        project "core-plugin-test4"        
            targetname "Test4"
            kind "StaticLib"
            
            links "core-zpm"
            
            zpm.uses {
                "Zefiros-Software/CoreLib"
            }          
                    
            includedirs {
                "../core/include/",
                "../plugin/test4/include/"
                }						     			
                
            files { 
                "../plugin/include/**.hpp",
                "../plugin/include/**.h",
                "../plugin/test4/src/**.cpp"
                }
premake5 install-package --allow-install --allow-module  || exit /b 1
premake5 vs2015 --allow-install || exit /b 1
msbuild bsp/CoreLib.sln /property:Configuration=Release /property:Platform=Win32 || exit /b 1
::msbuild bsp/CoreLib.sln /property:Configuration=Debug /property:Platform=Win32 || exit /b 1
msbuild bsp/CoreLib.sln /property:Configuration=Release /property:Platform=x64 || exit /b 1
::msbuild bsp/CoreLib.sln /property:Configuration=Debug /property:Platform=x64 || exit /b 1

cd test/
premake5 vs2015 --allow-install || exit /b 1
msbuild zpm/CoreLib-ZPM.sln || exit /b 1

cd ../

bin\x86\core-test.exe || exit /b 1
::bin\x86\core-testd.exe || exit /b 1

bin\x86_64\core-test.exe || exit /b 1
::bin\x86_64\core-testd.exe || exit /b 1

test\bin\x86\core-zpm-test.exe || exit /b 1
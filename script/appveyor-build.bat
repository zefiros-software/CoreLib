zpm install-package --allow-install --allow-module  || exit /b 1
zpm vs2015 --allow-install --ignore-updates || exit /b 1
msbuild core/CoreLib.sln /property:Configuration=Release /property:Platform=Win32 || exit /b 1
::msbuild core/CoreLib.sln /property:Configuration=Debug /property:Platform=Win32 || exit /b 1
msbuild core/CoreLib.sln /property:Configuration=Release /property:Platform=x64 || exit /b 1
::msbuild core/CoreLib.sln /property:Configuration=Debug /property:Platform=x64 || exit /b 1

cd test/
zpm vs2015 --allow-install || exit /b 1
msbuild zpm/CoreLib-ZPM.sln || exit /b 1

cd ../

bin\x86\core-test.exe || exit /b 1
::bin\x86\core-testd.exe || exit /b 1

bin\x86_64\core-test.exe || exit /b 1
::bin\x86_64\core-testd.exe || exit /b 1

test\bin\x86\core-zpm-test.exe || exit /b 1
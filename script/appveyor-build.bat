
if %TYPE% == "zpm" (
    cd test
    
    zpm install-package --allow-install --allow-module || exit /b 1
    zpm vs2015 --allow-install
    
    msbuild zpm/CoreLib-ZPM.sln || exit /b 1

    test\bin\x86\core-zpm-test.exe || exit /b 1
) else (
    zpm install-package --allow-install --allow-module  || exit /b 1
    zpm %VSTUD% --allow-install || exit /b 1

    msbuild core/CoreLib.sln /property:Configuration=Release /property:Platform=%PLAT% || exit /b 1

    if %TYPE% == "debug" (
        bin\%ARCH%\core-testd
    ) else (
        bin\%ARCH%\core-test
    )
)
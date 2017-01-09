set -e


if [ "$TYPE" == "zpm" ]; then
    cd test
    
    premake5 install-package --allow-install --allow-module
    premake5 gmake --allow-install --ignore-updates

    cd zpm/
    make
    cd ../../

    test/bin/${ARCH}/core-zpm-test

else
    premake5 install-package --allow-install --allow-module
    premake5 gmake --allow-install --ignore-updates
    cd core
    make config=${TYPE}_${ARCH}
    cd ../


    if [ "$TYPE" == "debug" ]; then
        bin/${ARCH}/core-testd

    elif [ "$TYPE" == "coverage" ]; then
        core-testcd
    else
        bin/${ARCH}/core-test
    fi
fi
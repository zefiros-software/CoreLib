set -e

premake5 install-package --allow-install --allow-module
premake5 gmake --allow-install --ignore-updates
cd core
make config=${TYPE}_${ARCH}
cd ../


if [ "$TYPE" == "debug" ]; then
    bin/${ARCH}/core-testd
elif [ "$TYPE" == "coverage" ]; then
    bin/${ARCH}/core-testcd
else
    bin/${ARCH}/core-test

    cd test
    
    premake5 gmake --allow-install

    cd zpm/
    make
    cd ../../

    test/bin/x86/core-zpm-test
fi

#!/bin/bash
set -e

if [ "$TYPE" == "zpm" ]; then
    cd test
    
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install --ignore-updates

    cd zpm/
    make
    cd ../../

    test/bin/x86/core-zpm-test

else
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install --ignore-updates
    cd core
    make config=${TYPE}_${ARCH}
    cd ../


    if [ "$TYPE" == "debug" ]; then
        bin/${ARCH}/core-testd

    elif [ "$TYPE" == "coverage" ]; then
        ./core-testcd
    else
        bin/${ARCH}/core-test
    fi
fi
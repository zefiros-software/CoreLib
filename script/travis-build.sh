#!/bin/bash
set -e

if [ "$TYPE" == "zpm" ]; then
    cd test
    
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install

    cd zpm/
    make
    cd ../../

    test/bin/x86/core-zpm-test

else
    zpm install-package --allow-install --allow-module
    zpm gmake --allow-install
    cd core
    make config=${TYPE}_${ARCH}
    cd ../


    if [ "$TYPE" == "debug" ]; then
        ASAN_OPTIONS=detect_odr_violation=0 bin/${ARCH}/core-testd
    elif [ "$TYPE" == "coverage" ]; then
        ASAN_OPTIONS=detect_odr_violation=0 ./core-testcd
    else
        ASAN_OPTIONS=detect_odr_violation=0 bin/${ARCH}/core-test
    fi
fi

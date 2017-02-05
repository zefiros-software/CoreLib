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
        if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then 
            #bin/${ARCH}/core-testd
            lldb -o run bin/${ARCH}/core-testd
        else
            bin/${ARCH}/core-testd
        fi

    elif [ "$TYPE" == "coverage" ]; then
        ./core-testcd
    else
        bin/${ARCH}/core-test
    fi
fi
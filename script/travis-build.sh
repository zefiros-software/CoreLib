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
            ulimit -c unlimited -S
            bin/${ARCH}/core-testd & pid=$! && fg;
            lldb --core /cores/core.$pid --batch --one-line "bt"
        else
            bin/${ARCH}/core-testd
        fi

    elif [ "$TYPE" == "coverage" ]; then
        ./core-testcd
    else
        bin/${ARCH}/core-test
    fi
fi
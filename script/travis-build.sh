set -e

premake5 install-package --allow-install --allow-module
premake5 gmake
cd core
make config=debug_x86
make config=debug_x86_64
make config=release_x86
make config=release_x86_64
#make config=coverage_x86
#make config=coverage_x86_64

cd ../test/

premake5 gmake

cd zpm/
make
cd ../../

bin/x86/core-test
bin/x86/core-testd
#bin/x86/core-testcd

bin/x86_64/core-test
bin/x86_64/core-testd
#bin/x86_64/core-testcd

test/bin/x86/core-zpm-test
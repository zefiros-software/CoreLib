set -e

cd bsp
make config=coverage_x86
cd ../

./coverage-testcd

codecov
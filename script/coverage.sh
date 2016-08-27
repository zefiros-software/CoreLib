set -e

cd core
make config=coverage_x86
cd ../

./coverage-testcd

codecov
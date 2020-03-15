set -x
set -e

if [ ! -d "vcpkg" ]
then
    git clone https://github.com/Microsoft/vcpkg.git
fi

pushd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install cairo
popd

# https://pycairo.readthedocs.io/en/latest/
pip install pycairo
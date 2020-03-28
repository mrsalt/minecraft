set -x
set -e

if [ ! -d "vcpkg" ]
then
    git clone https://github.com/Microsoft/vcpkg.git
fi

arch=x64
os=windows

pushd vcpkg
if [ ! -x "vcpkg.exe" ]
then
    ./bootstrap-vcpkg.sh
fi
./vcpkg install cairo:$arch-$os
popd

# https://pycairo.readthedocs.io/en/latest/
pip install pycairo
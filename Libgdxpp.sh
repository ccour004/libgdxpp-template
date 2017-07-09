
echo Executing Pre Build commands ...
mkdir build
cd build
cmake ..
ln -s ../assets assets
make
./Core

echo Done
make build

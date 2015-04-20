mkdir -p libs
cd ./libs
wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip -e gtest-1.7.0.zip
cd gtest-1.7.0
mkdir -p build
cd build
cmake ../
make -j2
cd ../../../


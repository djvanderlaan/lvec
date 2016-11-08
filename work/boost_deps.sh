#!/bin/sh

# This script downloads the boost source and copies all needed headers and source
# files into the folder boost_needed. The subfolders in this directoy can be 
# copied to replace the corresponding directories in ../src

# Download boost and unpack
wget https://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.bz2
tar -xjf boost_1_58_0.tar.bz2

# Use bcp to copy all required headers and source files to boost_needed
rm -rf boost_needed
mkdir boost_needed
bcp --scan --boost=./boost_1_58_0 ../src/*.cpp ../src/*.h boost_needed

# The source files are spread in a large number of subdirectories in libs; copy
# them all into the folder boost_src
mkdir boost_needed/boost_src
find ./boost_needed/libs -name '*.cpp' -exec cp {} ./boost_needed/boost_src \;
rm -rf ./boost_needed/libs


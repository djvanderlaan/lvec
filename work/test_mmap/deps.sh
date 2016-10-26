#!/bin/sh

rm -rf boost_needed
mkdir boost_needed
bcp --scan --boost=./boost_1_62_0 *.cpp *.h boost_needed

mkdir boost_needed/boost_src
find ./boost_needed/libs -name '*.cpp' -exec cp {} ./boost_needed/boost_src \;

#rm -rf ./boost_needed/libs


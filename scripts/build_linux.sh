#!/bin/sh

BUILD_PATH=`pwd`
echo ${BUILD_PATH}
cd ${BUILD_PATH}/../third_party/SURFACE/;
git submodule update --init
cd ${BUILD_PATH}/../third_party/lua/;
git submodule update --init
cd ${BUILD_PATH}

cmake ../
make
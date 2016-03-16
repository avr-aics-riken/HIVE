#!/bin/bash

## -- Assume $HIVE is the root directory.
BINARY_DIR=build/bin/
## ----

VERSIONSTR=master
OSNAME=dev

# Use tag as version
if [ $TRAVIS_TAG ]; then
  VERSIONSTR=$TRAVIS_TAG
fi

# Use tag as version
if [ $TRAVIS_OS_NAME ]; then
  OSNAME=$TRAVIS_OS_NAME
fi

# We create 64bit binary only.
rm -rf dist
mkdir dist
tar -v -c "${BINARY_DIR}" > dist/tmp.tar

# rename directory & create tar.gz
(cd dist; rm -rf HIVE-"${VERSIONSTR}"-"${OSNAME}"64; mkdir HIVE-"${VERSIONSTR}-"${OSNAME}"64")
(cd dist; tar xvf tmp.tar -C HIVE-"${VERSIONSTR}"-"${OSNAME}"64 --strip-components 2)
(cd dist; tar -c HIVE-"${VERSIONSTR}"-"${OSNAME}"64 > HIVE-"${VERSIONSTR}"-"${OSNAME}"64.tar)
(cd dist; gzip HIVE-"${VERSIONSTR}"-"${OSNAME}"64.tar)
(cd dist; rm -rf HIVE-"${VERSIONSTR}"-"${OSNAME}"64)
(cd dist; rm -rf tmp.tar)

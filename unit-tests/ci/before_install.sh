#!/bin/bash

phpenv rehash
git submodule --quiet update --init --recursive
cd json-c
git checkout patch-1
sh autogen.sh 2&>1 /dev/null
./configure --silent --prefix=$TRAVIS_BUILD_DIR/build
make --silent
make --silent install

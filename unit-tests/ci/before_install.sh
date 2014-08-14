#!/bin/bash

phpenv rehash
git submodule --quiet update --init --recursive
cd json-c
git checkout patch-1
sh autogen.sh 2&>1 /dev/null
./configure --silent
make --silent
sudo make --silent install

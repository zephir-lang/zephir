#!/bin/bash

phpenv rehash
sudo apt-get update
sudo apt-get -qq install re2c valgrind
git submodule --quiet update --init --recursive
cd json-c
git checkout patch-1
sh autogen.sh 2&>1 /dev/null
./configure --silent
make --silent
sudo make --silent install

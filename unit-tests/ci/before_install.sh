#!/bin/bash

phpenv rehash
sudo apt-get update
sudo apt-get -qq install re2c valgrind

git clone git://github.com/zephir-lang/parser.git ext-parser
cd ext-parser
export CFLAGS="-g3 -O1 -fno-delete-null-pointer-checks -Wall -fvisibility=hidden"
make lemon
make parser
make extension
echo 'extension="zephir_parser.so"' >> ~/.phpenv/versions/$(phpenv version-name)/etc/conf.d/travis.ini
cd ../..

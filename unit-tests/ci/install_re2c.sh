#!/usr/bin/env bash

echo -e "Install a tool for writing fast and flexible scanners in C from regular expressions"

if [ "${CI}" != "true" ]; then
    echo "This script is designed to run inside a CI container only. Exiting..."
    exit 1
fi

if [ -z ${RE2C_VERSION+x} ]; then
    echo "The RE2C_VERSION is unset. Exit..."
    exit 1
fi

if [ "${RE2C_VERSION}" == "system" ]; then
    echo "Use system re2c. Skip..."
    exit 0
fi

pkgname=re2c
pkgver=${RE2C_VERSION}
source="https://github.com/skvadrik/$pkgname/releases/download/$pkgver/$pkgname-$pkgver.tar.gz"
builddir="${HOME}/.cache/$pkgname/$pkgname-$pkgver"
bindir="$builddir/bin"

if [ ! -f "${bindir}/re2c" ]; then
    mkdir -p "${HOME}/.cache/$pkgname"
    cd "${HOME}/.cache/$pkgname"

    rm -f "$pkgname-$pkgver.tar.gz"
    rm -rf ./"$pkgname-$pkgver"

    curl -sSL "$source" -o "$pkgname-$pkgver.tar.gz"
    tar -zxf "$pkgname-$pkgver.tar.gz"

    cd ./"$pkgname-$pkgver"
    ./configure --prefix=${builddir}

    make -j"$(getconf _NPROCESSORS_ONLN)"
    make install
fi

mkdir -p ${HOME}/bin
ln -s "$bindir/re2c" ${HOME}/bin/re2c

re2c --version

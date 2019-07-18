#!/bin/sh
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

# -e  Exit immediately if a command exits with a non-zero status.
# -u  Treat unset variables as an error when substituting.
set -eu

if [ -z ${CI+x} ] || [ "$CI" != "true" ]; then
  >&2 printf "This script is designed to run inside a CI container only.\nAborting.\n"
  exit 1
fi

if [ -z ${RE2C_VERSION+x} ]; then
  >&2 printf "The RE2C_VERSION is unset.\nAborting.\n"
  exit 1
fi

if [ "${RE2C_VERSION}" = "system" ]; then
  printf "Use system re2c.\nSkip.\n"
  exit 0
fi

pkgname=re2c
source="https://github.com/skvadrik/${pkgname}/releases/download/${RE2C_VERSION}/${pkgname}-${RE2C_VERSION}.tar.gz"
downloaddir="${HOME}/.cache/${pkgname}/${pkgname}-${RE2C_VERSION}"
prefix="${HOME}/.local/opt/${pkgname}/${pkgname}-${RE2C_VERSION}"
bindir="${prefix}/bin"

if [ ! -f "${bindir}/re2c" ]; then
  if [ ! -d `dirname ${downloaddir}` ]; then
    mkdir -p `dirname ${downloaddir}`
  fi

  cd `dirname ${downloaddir}`

  if [ ! -f "${pkgname}-${RE2C_VERSION}.tar.gz" ]; then
    curl -sSL "$source" -o "${pkgname}-${RE2C_VERSION}.tar.gz"
  fi

  if [ ! -f "${pkgname}-${RE2C_VERSION}.tar.gz" ]; then
    >&2 printf "Aborting.\n"
    >&2 printf "Unable to locate ${pkgname}-${RE2C_VERSION}.tar.gz file.\n"
    exit 1
  fi

  if [ ! -d "${downloaddir}" ]; then
    mkdir -p "${downloaddir}"
    tar -zxf "${pkgname}-${RE2C_VERSION}.tar.gz"
  fi

  if [ ! -d "${downloaddir}" ]; then
    >&2 printf "Unable to locate re2c source.\nAborting.\n"
    exit 1
  fi

  if [ ! -d "${prefix}" ]; then
    mkdir -p "${prefix}"
  fi

  cd "${downloaddir}"
  ./configure --prefix="${prefix}"

  make -j"$(getconf _NPROCESSORS_ONLN)"
  make install
fi

if [ ! -x "${bindir}/re2c" ]; then
  >&2 printf "Unable to locate re2c executable.\nAborting.\n"
  exit 1
fi

cd ${TRAVIS_BUILD_DIR}

mkdir -p ${HOME}/bin
ln -s "${bindir}/re2c" ${HOME}/bin/re2c

re2c --version
exit 0

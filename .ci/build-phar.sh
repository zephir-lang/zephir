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

project_root=$(readlink -enq "$(dirname $0)/../")

if [ $(command -v box 2>/dev/null) = "" ]; then
  >&2 printf "To use this script you need to install humbug/box.\n"
  >&2 printf "You can learn all about humbug/box on https://github.com/humbug/box.\n"
  >&2 printf "Aborting.\n"
  exit 1
fi

box compile --working-dir=${project_root}

if [ ! -f "${project_root}/zephir.phar" ] || [ ! -x "${project_root}/zephir.phar" ]; then
  >&2 printf "Something went wrong when building Zephir.\n"
  >&2 printf "Aborting.\n"
  exit 1
fi

mkdir -p ${HOME}/bin
rm -f ${HOME}/bin/zephir

ln -s "${project_root}/zephir.phar" ${HOME}/bin/zephir

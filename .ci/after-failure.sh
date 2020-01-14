#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Phalcon Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

autoconf --version
echo "----------------------------------------"

cc --version
echo "----------------------------------------"

make --version
echo "----------------------------------------"

re2c --version
echo "----------------------------------------"

php -v
echo "----------------------------------------"

phpize --version
echo "----------------------------------------"

php -m
echo "----------------------------------------"

comand -v php
echo "----------------------------------------"

comand -v php-config
echo "----------------------------------------"

comand -v phpize
echo "----------------------------------------"

php-config
echo "----------------------------------------"

ls -al "$(php-config --extension-dir)"
echo "----------------------------------------"

if [ -f ./compile-errors.log ]
then
  log_contents=$(cat ./compile-errors.log)
  [[ -z "${log_contents// }" ]] || {
    (>&1 echo "Compiler log:")
    (>&1 printf "%s\\n" "$log_contents")
  }
fi

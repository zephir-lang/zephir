#!/usr/bin/env bash

# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

# set sharness verbosity. we set the env var directly as
# it's too late to pass in --verbose, and --verbose is harder
# to pass through in some cases.
# shellcheck disable=SC2034
test "$TEST_VERBOSE" = 1 && verbose=t

readonly SHARNESS_LIB="./lib/sharness/sharness.sh"

# shellcheck source=lib/sharness/sharness.sh
source "$SHARNESS_LIB" || {
  echo >&2 "Cannot source: $SHARNESS_LIB"
  echo >&2 "Please check Sharness installation."
  exit 1
}

if test "$TEST_VERBOSE" = 1; then
  echo '[DEBUG] TEST_VERBOSE='"$TEST_VERBOSE"
fi

# shellcheck disable=SC2155
export ZEPHIRDIR=$(cd "$(dirname "$0")/../../../" || exut 1; pwd)

# shellcheck disable=SC2155
export TESTSDIR=$(cd "$(dirname "$0")/../../" || exut 1; pwd)
export FIXTURESDIR="$TESTSDIR/fixtures"
export OUTPUTDIR="$TESTSDIR/output"
export ZEPHIR_BIN="$ZEPHIRDIR/zephir"
# shellcheck disable=SC2155
export PHP_VERSION_ID=$(php -r 'echo PHP_VERSION_ID;')

: "${PHP:=$(command -v php 2>/dev/null)}"

# shellcheck disable=SC2139
function zephirc() {
  test "$TEST_VERBOSE" = 1 && (>&1 printf "[DEBUG4] %s\n" "$PHP $ZEPHIR_BIN $*")
  eval "$PHP $ZEPHIR_BIN $*"
}

function cleanup_output() {
  [ -n "$1" ] || return 0

  for f in $1; do
    if test -f "$OUTPUTDIR/$f"
    then
      test "$TEST_VERBOSE" = 1 && (>&1 printf "[DEBUG2] rm -f %s\n" "$OUTPUTDIR/$f")
      rm -f "$OUTPUTDIR/$f"
    elif test -d "$OUTPUTDIR/$f"
    then
      test "$TEST_VERBOSE" = 1 && (>&1 printf "[DEBUG1] rm -rf %s\n" "$OUTPUTDIR/$f")
      rm -rf "$OUTPUTDIR/${f:?}"
    fi
  done
}

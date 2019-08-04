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
  (>&1 printf "# TEST_VERBOSE='%s'\n" "$TEST_VERBOSE")
fi

ZEPHIRDIR=$(cd "$(dirname "$0")/../../../" || exit 1; pwd)
TESTSDIR=$(cd "$(dirname "$0")/../../" || exit 1; pwd)
export ZEPHIRDIR TESTSDIR

FIXTURESDIR="$TESTSDIR/fixtures"
OUTPUTDIR="$TESTSDIR/output"
ZEPHIR_BIN="$ZEPHIRDIR/zephir"
PHP_VERSION_ID=$(php -r 'echo PHP_VERSION_ID;')
export FIXTURESDIR OUTPUTDIR ZEPHIR_BIN PHP_VERSION_ID

: "${PHP:=$(command -v php 2>/dev/null)}"

function zephirc() {
  test "$TEST_VERBOSE" = 1 && (>&1 printf "# %s\\n" "$PHP $ZEPHIR_BIN $*")
  eval "$PHP $ZEPHIR_BIN $*"
}

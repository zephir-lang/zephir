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
  echo '# TEST_VERBOSE='"$TEST_VERBOSE"
fi

# shellcheck disable=SC2155
export ZEPHIRDIR=$(cd "$(dirname $0)/../../../"; pwd)

# shellcheck disable=SC2155
export TESTSDIR=$(cd "$(dirname $0)/../../"; pwd)
export FIXTURESDIR="$TESTSDIR/fixtures"
export OUTPUTDIR="$TESTSDIR/output"
export ZEPHIR_BIN="$ZEPHIRDIR/zephir"
# shellcheck disable=SC2155
export PHP_VERSION_ID=$(php -r 'echo PHP_VERSION_ID;')

# shellcheck disable=SC2139
alias zephir="php $ZEPHIR_BIN"

function cleanup_output() {
  [ -n "$1" ] || return 0

  for f in $1; do
    test -f "$OUTPUTDIR/$f" && {
      test "$TEST_VERBOSE" = 1 && (>&1 printf "rm -f %s\n" "$OUTPUTDIR/$f")
      rm -f "$OUTPUTDIR/$f"
    }
  done
}

# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

# set sharness verbosity. we set the env var directly as
# it's too late to pass in --verbose, and --verbose is harder
# to pass through in some cases.
test "$TEST_VERBOSE" = 1 && verbose=t

SHARNESS_LIB="./lib/sharness/sharness.sh"

. "$SHARNESS_LIB" || {
	echo >&2 "Cannot source: $SHARNESS_LIB"
	echo >&2 "Please check Sharness installation."
	exit 1
}

if test "$TEST_VERBOSE" = 1; then
	echo '# TEST_VERBOSE='"$TEST_VERBOSE"
fi

export ZEPHIRDIR=$(cd "$(dirname $0)/../../../"; pwd)
export TESTSDIR=$(cd "$(dirname $0)/../../"; pwd)
export FIXTURESDIR="$TESTSDIR/fixtures"
export OUTPUTDIR="$TESTSDIR/output"

export ZEPHIR_BIN="${ZEPHIRDIR}/zephir"

: ${PHP:=php}

export PHP_VERSION_ID=$(${PHP} -r 'echo PHP_VERSION_ID;')

alias zephir="${PHP} ${ZEPHIR_BIN}"

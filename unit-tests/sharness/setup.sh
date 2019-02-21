. ./lib/sharness/sharness.sh

export ZEPHIRDIR=$(cd "$(dirname $0)/../../../"; pwd)
export TESTSDIR=$(cd "$(dirname $0)/../../"; pwd)
export FIXTURESDIR="$TESTSDIR/fixtures"

export ZEPHIR_BIN="${ZEPHIRDIR}/zephir"

if [ x"$PHP" = x ]; then
	PHP=php
fi

export PHP_VERSION_ID=$($PHP -r 'echo PHP_VERSION_ID;')

alias zephir="${PHP} ${ZEPHIR_BIN}"

#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Phalcon Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.
#
# Runs the .zept BlackBox suite (PHPUnit @group zept) split across N parallel
# shards. Each shard builds a disjoint subset of cases (see
# ZeptSuiteTest::zeptFiles via the ZEPT_SHARD env), so wall-clock time drops
# roughly N-fold on a multi-core runner. Exits non-zero if any shard fails.
#
# Usage: .ci/run-zept-parallel.sh [num_shards]   (default: online CPU count)

set -euo pipefail

shards="${1:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}"
phpunit="vendor/bin/phpunit"
test_file="tests/Zephir/BlackBox/ZeptSuiteTest.php"

pids=()
logs=()

for ((i = 1; i <= shards; i++)); do
    log="$(mktemp "${TMPDIR:-/tmp}/zept-shard-${i}.XXXXXX")"
    logs+=("${log}")
    ZEPT_SHARD="${i}/${shards}" php "${phpunit}" "${test_file}" --no-coverage >"${log}" 2>&1 &
    pids+=("$!")
done

status=0
for idx in "${!pids[@]}"; do
    if ! wait "${pids[idx]}"; then
        status=1
    fi
done

for log in "${logs[@]}"; do
    cat "${log}"
    rm -f "${log}"
done

exit "${status}"

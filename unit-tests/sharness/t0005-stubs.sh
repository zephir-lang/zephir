#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generates IDE stubs"

# shellcheck disable=SC1091
source ./setup.sh

# shellcheck disable=SC2164,SC2069
# Generate Stubs for test
zephir_stubs() {
  cd "$FIXTURESDIR"/stubs/issues
  zephirc generate --no-ansi 2>&1 >/dev/null
  zephirc stubs --no-ansi 2>&1 >/dev/null
}

# Some stubs test requires PHP >= 7.1
if [ "$PHP_VERSION_ID" -lt "71000" ]; then
  test_set_prereq PHP70
fi

# This test generates Stubs once and all other tests uses this build artifacts
test_expect_success "Should generate Stubs" \
  "zephir_stubs && test -d ./ide/0.0.1/Stubs"

# See: https://github.com/phalcon/zephir/issues/1922
test_expect_success "Should properly generate type hint" \
  "test_cmp expected/Issue_1922.zep.php ide/0.0.1/Stubs/Issue_1922.zep.php"

# See: https://github.com/phalcon/zephir/issues/1778
test_expect_success "Should properly namespace imports (use block)" \
  "test_cmp expected/Issue_1778.zep.php ide/0.0.1/Stubs/Issue_1778.zep.php"

# See: https://github.com/phalcon/zephir/issues/1900
test_expect_success "Should properly generate return types for stubs" \
  "test_cmp expected/Issue_1900.zep.php ide/0.0.1/Stubs/Issue_1900.zep.php"

# See: https://github.com/phalcon/zephir/issues/1907
test_expect_success "Should properly generate Namespace for extends" \
  "test_cmp expected/Exception.zep.php ide/0.0.1/Stubs/Exception.zep.php"

# See: https://github.com/phalcon/zephir/issues/1907
test_expect_success "Should properly generate Namespace for extends (slash)" \
  "test_cmp expected/Issue_1907.zep.php ide/0.0.1/Stubs/Issue_1907.zep.php"

# See: https://github.com/phalcon/zephir/issues/1986
test_expect_success "Should properly generate Aliases for use statements" \
  "test_cmp expected/Issue_1986.zep.php ide/0.0.1/Stubs/Issue_1986.zep.php"

# See: https://github.com/phalcon/zephir/issues/1896
test_expect_success "Should generage CamelCase folders for stubs" \
  "test $(ls ./ide/0.0.1/Stubs/Events/ManagerInterface.zep.php | sed -e 's~\/~\\~g') = .\ide\0.0.1\Stubs\Events\ManagerInterface.zep.php"

# See: https://github.com/phalcon/zephir/issues/2026
test_expect_success "Should properly generate return type for Collections" \
  "test_cmp expected/Issue_2026.zep.php ide/0.0.1/Stubs/Issue_2026.zep.php"

if test_have_prereq PHP70; then
  say 'skipping generation stubs tests with return types not supported in PHP 7.0.x'
  cat ide/0.0.1/Stubs/Issue_2092.zep.php
  test_expect_success "Should properly generate return type for type hinted object" \
    "test_cmp expected/Issue_2092_PHP70.zep.php ide/0.0.1/Stubs/Issue_2092.zep.php"
else
  # See: https://github.com/phalcon/zephir/issues/2092
  test_expect_success "Should properly generate return type for type hinted object" \
    "test_cmp expected/Issue_2092.zep.php ide/0.0.1/Stubs/Issue_2092.zep.php"
fi

test_done

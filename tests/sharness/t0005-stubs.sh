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

# This test generates Stubs once and all other tests uses this build artifacts
test_expect_success "Should generate Stubs" \
  "zephir_stubs && test -d ./ide/0.0.1/Stubs"

# See: https://github.com/zephir-lang/zephir/issues/1922
test_expect_success "Should properly generate type hint" \
  "test_cmp expected/Issue_1922.php ide/0.0.1/Stubs/Issue_1922.php"

# See: https://github.com/zephir-lang/zephir/issues/1778
test_expect_success "Should properly namespace imports (use block)" \
  "test_cmp expected/Issue_1778.php ide/0.0.1/Stubs/Issue_1778.php"

# See: https://github.com/zephir-lang/zephir/issues/1900
test_expect_success "Should properly generate return types for stubs" \
  "test_cmp expected/Issue_1900.php ide/0.0.1/Stubs/Issue_1900.php"

# See: https://github.com/zephir-lang/zephir/issues/1907
test_expect_success "Should properly generate Namespace for extends" \
  "test_cmp expected/Exception.php ide/0.0.1/Stubs/Exception.php"

# See: https://github.com/zephir-lang/zephir/issues/1907
test_expect_success "Should properly generate Namespace for extends (slash)" \
  "test_cmp expected/Issue_1907.php ide/0.0.1/Stubs/Issue_1907.php"

# See: https://github.com/zephir-lang/zephir/issues/1986
test_expect_success "Should properly generate Aliases for use statements" \
  "test_cmp expected/Issue_1986.php ide/0.0.1/Stubs/Issue_1986.php"

# See: https://github.com/zephir-lang/zephir/issues/1896
test_expect_success "Should generate CamelCase folders for stubs" \
  "test $(ls ./ide/0.0.1/Stubs/Events/ManagerInterface.php | sed -e 's~\/~\\~g') = .\ide\0.0.1\Stubs\Events\ManagerInterface.php"

# See: https://github.com/zephir-lang/zephir/issues/2026
test_expect_success "Should properly generate return type for Collections" \
  "test_cmp expected/Issue_2026.php ide/0.0.1/Stubs/Issue_2026.php"

# See: https://github.com/zephir-lang/zephir/issues/2092
test_expect_success "Should properly generate return type for type hinted object" \
  "test_cmp expected/Issue_2092.php ide/0.0.1/Stubs/Issue_2092.php"

test_done

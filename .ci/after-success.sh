#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

#if [ "x${REPORT_COVERAGE}" = "x1" ]; then
#	lcov --no-checksum --directory ./ext --capture --compat-libtool --output-file ./coverage.info
#	lcov \
#		--remove ./coverage.info "/usr*" \
#		--remove ./coverage.info "*/.phpenv/*" \
#		--remove ./coverage.info "${HOME}/build/include/*" \
#		--compat-libtool \
#		--output-file ./coverage.info
#fi

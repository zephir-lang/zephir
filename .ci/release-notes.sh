#!/usr/bin/env bash

# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

# shellcheck disable=SC2002

# -e  Exit immediately if a command exits with a non-zero status.
# -u  Treat unset variables as an error when substituting.

set -eu
set -o pipefail

# Get Release notes for the latest release from CHANGELOG.md
# How to use:
#   release-notes.sh CHANGELOG.md > ReleaseNotes.md


startline=$(cat "$1" | grep -nE '^### ' | head -n 1 | cut -d ":" -f 1)
finishline=$(($(cat "$1" | grep -nE '^## \[[0-9]+' | head -n 2 | tail -n 1 | cut -d ":" -f 1) - 1))
changelog=$(sed -n "${startline},${finishline}p" "$1");

echo "${changelog}"

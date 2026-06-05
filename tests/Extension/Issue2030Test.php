<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue2030;

/**
 * Escape sequences in double-quoted strings must match PHP byte-for-byte.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2030
 */
final class Issue2030Test extends TestCase
{
    public function testEscapeSequencesMatchPhp(): void
    {
        $test = new Issue2030();

        $this->assertSame("\e[0;31m", $test->escapeChar());
        $this->assertSame("\u{41}", $test->unicodeAscii());
        $this->assertSame("\u{E9}", $test->unicode2Byte());
        $this->assertSame("\u{20AC}", $test->unicode3Byte());
        $this->assertSame("\u{1F600}", $test->unicode4Byte());

        // \x41 = 'A' then literal "BC" (PHP reads at most 2 hex digits).
        $this->assertSame("\x41BC", $test->hexFollowedByHex());
        $this->assertSame('ABC', $test->hexFollowedByHex());
    }
}

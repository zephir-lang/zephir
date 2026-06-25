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

namespace Zephir\Test\Zept;

use PHPUnit\Framework\TestCase;
use Zephir\Zept\OutputMatcher;

final class OutputMatcherTest extends TestCase
{
    private OutputMatcher $matcher;

    protected function setUp(): void
    {
        $this->matcher = new OutputMatcher();
    }

    public function testExactMatchIgnoresTrailingWhitespace(): void
    {
        $this->assertTrue($this->matcher->matches("hello\nworld", "hello\nworld\n", false));
    }

    public function testExactMismatchFails(): void
    {
        $this->assertFalse($this->matcher->matches('hello', 'goodbye', false));
    }

    public function testFormatDigitsSpecifier(): void
    {
        $this->assertTrue($this->matcher->matches('id=%d', 'id=42', true));
        $this->assertFalse($this->matcher->matches('id=%d', 'id=x', true));
    }

    public function testFormatStringSpecifierStopsAtNewline(): void
    {
        $this->assertTrue($this->matcher->matches('name=%s', 'name=zephir', true));
        // %s must not span a newline.
        $this->assertFalse($this->matcher->matches('name=%s', "name=a\nb", true));
    }

    public function testFormatAnySpecifierSpansNewlines(): void
    {
        $this->assertTrue($this->matcher->matches('start%aend', "start\nmiddle\nend", true));
    }

    public function testFormatEscapesLiteralRegexMetacharacters(): void
    {
        // Parentheses and '+' are literal text, not regex operators.
        $this->assertTrue($this->matcher->matches('(1+2)=%d', '(1+2)=3', true));
        $this->assertFalse($this->matcher->matches('(1+2)=%d', '112=3', true));
    }

    public function testFormatRawRegexSegment(): void
    {
        $this->assertTrue($this->matcher->matches('color=%r(red|blue)%r!', 'color=blue!', true));
        $this->assertFalse($this->matcher->matches('color=%r(red|blue)%r!', 'color=green!', true));
    }
}

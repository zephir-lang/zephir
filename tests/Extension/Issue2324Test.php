<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue2324;
use Stub\Issue2324Extended;

/**
 * Regression coverage for `new static()` — late static binding must resolve
 * to the *called* class, not the lexical class. `new self()` continues to
 * resolve to the lexical class.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2324
 */
final class Issue2324Test extends TestCase
{
    public function testNewStaticOnBaseClassReturnsBaseClass(): void
    {
        $this->assertSame(Issue2324::class, Issue2324::makeStatic());
    }

    public function testNewStaticOnSubclassReturnsSubclass(): void
    {
        $this->assertSame(Issue2324Extended::class, Issue2324Extended::makeStatic());
    }

    public function testNewSelfAlwaysReturnsLexicalClass(): void
    {
        $this->assertSame(Issue2324::class, Issue2324::makeSelf());
        $this->assertSame(Issue2324::class, Issue2324Extended::makeSelf());
    }

    public function testNewStaticFromInstanceMethodHonoursActualClass(): void
    {
        $base = new Issue2324();
        $sub  = new Issue2324Extended();

        $this->assertSame(Issue2324::class, $base->makeStaticInstance());
        $this->assertSame(Issue2324Extended::class, $sub->makeStaticInstance());
    }
}

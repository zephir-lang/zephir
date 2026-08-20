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

namespace Zephir\Test\Parser;

use PHPUnit\Framework\TestCase;
use Zephir\Parser\Manager;

/**
 * An outdated `ext-zephir_parser` must NOT be used as the fast path, because
 * its grammar is older than the syntax the compiler accepts: 2.0.x silently
 * dropped trait `use` statements (zephir#504) and everything before 2.7.0
 * rejects an expression in a constant initializer or property default outright
 * (zephir#2061). The built-in pure-PHP parser takes over instead.
 */
final class ManagerTest extends TestCase
{
    public function testExtensionIsNotUsableWhenAbsent(): void
    {
        $this->assertFalse(Manager::isExtensionUsable(null));
    }

    public function testOutdatedExtensionIsNotUsable(): void
    {
        $this->assertFalse(Manager::isExtensionUsable('2.6.0'));
        $this->assertFalse(Manager::isExtensionUsable('2.0.4'));
        $this->assertFalse(Manager::isExtensionUsable('1.5.0'));
    }

    public function testCurrentAndNewerExtensionsAreUsable(): void
    {
        $this->assertTrue(Manager::isExtensionUsable(Manager::MINIMUM_PARSER_VERSION));
        $this->assertTrue(Manager::isExtensionUsable('2.7.0'));
        $this->assertTrue(Manager::isExtensionUsable('2.8.0'));
        $this->assertTrue(Manager::isExtensionUsable('3.0.0'));
    }

    public function testTheMinimumTracksTheNewestGrammarFeature(): void
    {
        // Guards against raising a grammar feature without raising the floor:
        // the released parser that introduced it is the minimum.
        $this->assertSame('2.7.0', Manager::MINIMUM_PARSER_VERSION);
    }
}

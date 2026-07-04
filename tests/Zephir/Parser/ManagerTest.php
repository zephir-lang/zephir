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
 * An outdated `ext-zephir_parser` must NOT be used as the fast path: parsers
 * older than 2.1.0 predate trait AST nodes and silently drop unknown class
 * members, so trait `use` statements would be lost without any error. The
 * built-in pure-PHP parser takes over instead (zephir#504).
 */
final class ManagerTest extends TestCase
{
    public function testExtensionIsNotUsableWhenAbsent(): void
    {
        $this->assertFalse(Manager::isExtensionUsable(null));
    }

    public function testOutdatedExtensionIsNotUsable(): void
    {
        $this->assertFalse(Manager::isExtensionUsable('2.0.4'));
        $this->assertFalse(Manager::isExtensionUsable('1.5.0'));
    }

    public function testCurrentAndNewerExtensionsAreUsable(): void
    {
        $this->assertTrue(Manager::isExtensionUsable(Manager::MINIMUM_PARSER_VERSION));
        $this->assertTrue(Manager::isExtensionUsable('2.1.0'));
        $this->assertTrue(Manager::isExtensionUsable('2.2.0'));
        $this->assertTrue(Manager::isExtensionUsable('3.0.0'));
    }
}

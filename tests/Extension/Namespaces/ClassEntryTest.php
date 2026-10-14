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

namespace Extension\Namespaces;

use PHPUnit\Framework\TestCase;
use Stub\Namespaces\A\B\Sub;
use Stub\Namespaces\ClassEntry;

final class ClassEntryTest extends TestCase
{
    public function testCorrectlyDetectClassEntryFromParam(): void
    {
        $subClass = new Sub();
        $class = new ClassEntry();

        $this->assertInstanceOf(Sub::class, $class->setParam($subClass));
        $this->assertInstanceOf(Sub::class, $class->setParamImported($subClass));
    }

    /**
     * Unqualified names in a closure resolve against the file namespace.
     *
     * @issue https://github.com/zephir-lang/zephir/issues/2686
     */
    public function testIssue2686ClosureResolvesSubNamespace(): void
    {
        $sub = new Sub();

        $this->assertInstanceOf(Sub::class, ($sub->issue2686ClosureNew())());
        $this->assertTrue(($sub->issue2686ArrowInstanceOf())($sub));
        $this->assertFalse(($sub->issue2686ArrowInstanceOf())(new \stdClass()));
    }
}

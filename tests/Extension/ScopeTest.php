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
use Stub\Scope;

final class ScopeTest extends TestCase
{
    public function testScope1(): void
    {
        if (version_compare(PHP_VERSION, '8.2.0', '<')) {
            $this->assertSame(Scope::test1(), 'testinternal_string');
            $this->assertSame(Scope::test3(), 'internal_0internal_1internal_2internal_3');
        }

        $this->assertSame(Scope::test2(), [15, '0_66_132_198_']);
    }
}

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

namespace Zephir\Test\Variable;

use PHPUnit\Framework\TestCase;
use Zephir\Variable\Globals;

final class GlobalsTest extends TestCase
{
    public function testIsSuperGlobal(): void
    {
        $globals = new Globals();

        $this->assertTrue($globals->isSuperGlobal('_GET'));
        $this->assertTrue($globals->isSuperGlobal('_POST'));
        $this->assertTrue($globals->isSuperGlobal('_COOKIE'));
        $this->assertTrue($globals->isSuperGlobal('_SERVER'));
        $this->assertTrue($globals->isSuperGlobal('_ENV'));
        $this->assertTrue($globals->isSuperGlobal('_FILES'));
        $this->assertTrue($globals->isSuperGlobal('_REQUEST'));
        $this->assertFalse($globals->isSuperGlobal('foo'));
    }
}

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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * Named arguments and the arguments call_user_func() forwards are passed in
 * an array the generated C builds with the kernel's array functions, so the
 * file has to `#include "kernel/array.h"` even when nothing else in it uses
 * an array, or a compiler that rejects implicit declarations (clang) fails.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
final class Issue2824ArrayHeaderTest extends TestCase
{
    use GeneratesStubProject;

    protected function setUp(): void
    {
        $this->setUpStubProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testNamedArgumentsIncludeTheArrayHeader(): void
    {
        $generated = $this->generateOk(
            'named.zep',
            <<<'ZEP'
                namespace Stub;

                class Named
                {
                    public function pair(var a, var b) -> string
                    {
                        return a . b;
                    }

                    public function swapped() -> string
                    {
                        return this->pair(b: "2", a: "1");
                    }
                }
                ZEP,
        );

        $this->assertStringContainsString('zephir_array_update_string', $generated);
        $this->assertStringContainsString('#include "kernel/array.h"', $generated);
    }

    public function testCallUserFuncArgumentsIncludeTheArrayHeader(): void
    {
        $generated = $this->generateOk(
            'forwards.zep',
            <<<'ZEP'
                namespace Stub;

                class Forwards
                {
                    public function run(var callback) -> string
                    {
                        return call_user_func(callback, "a", "b");
                    }
                }
                ZEP,
        );

        $this->assertStringContainsString('zephir_create_array', $generated);
        $this->assertStringContainsString('#include "kernel/array.h"', $generated);
    }
}

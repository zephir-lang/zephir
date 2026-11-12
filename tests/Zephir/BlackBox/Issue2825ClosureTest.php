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

use function file_get_contents;
use function glob;
use function implode;

/**
 * A closure resolves `self`, `parent` and `static` as the code it is written
 * in, passes that code's object on to `self::`/`static::` calls, and is named
 * as PHP names it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
final class Issue2825ClosureTest extends TestCase
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

    public function testFunctionInClosureIsTheNamePhpGivesIt(): void
    {
        $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names
            {
                public function make() -> var
                {
                    return function () {
                        return __FUNCTION__;
                    };
                }
            }
            ZEP);

        $this->assertStringContainsString(
            'zephir_closure_name(return_value, SL("Stub\\\\{closure}"), SL("{closure:Stub\\\\Names::make():7}"));',
            $this->closureSources()
        );
    }

    public function testClassScopeCallInCapturingClosurePassesTheEnclosingObject(): void
    {
        $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names
            {
                public function im() -> int { return 1; }

                public function make() -> var
                {
                    var x = 1;

                    return function () use (x) { return self::im() + x; };
                }
            }
            ZEP);

        $closure = $this->closureSources();
        $this->assertStringContainsString('this_ptr = &__$zephir_this;', $closure);
        $this->assertStringContainsString('ZEPHIR_CALL_SELF_THIS_CACHED(&_0, this_ptr, "im"', $closure);
    }

    public function testSelfInAFunctionIsACompileError(): void
    {
        $result = $this->generate(['names.zep' => <<<'ZEP'
            namespace Stub;

            function selfName() -> var { return self::C; }

            class Names
            {
                const C = 1;
            }
            ZEP]);

        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString(
            'Cannot use "self" when no class scope is active',
            $result['stdout'] . $result['stderr']
        );
    }

    private function closureSources(): string
    {
        $sources = [];
        foreach (glob($this->projectDir . '/ext/stub/*__closure.zep.c') ?: [] as $file) {
            $sources[] = (string) file_get_contents($file);
        }

        return implode("\n", $sources);
    }
}

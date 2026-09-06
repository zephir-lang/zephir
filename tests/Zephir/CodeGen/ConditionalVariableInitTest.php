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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;

/**
 * A local whose only assignment sits inside a conditional has to be registered
 * with the memory frame at its declaration, so a path that skips the
 * assignment reads null the way PHP does.
 *
 * The registration is one ZEPHIR_INIT_VAR at the top of the body rather than
 * an IS_NULL in the declaration block, because IS_UNDEF at declaration is what
 * makes a first write observe the slot. With the slot already registered every
 * later write reads Variable::$variantInits and downgrades itself to
 * ZEPHIR_INIT_NVAR or ZEPHIR_OBS_NVAR, which observe only while the target is
 * still undefined, so it is registered exactly once. Registering it twice is
 * not a leak but an abort: see kernel/memory.c zephir_do_memory_observe().
 *
 * The guard tests matter as much as the positive ones. Every extra
 * registration forces a memory frame on a method that may not have needed one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
final class ConditionalVariableInitTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_issue2679_test_', ['stub/issue2679']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    /**
     * Each case names the local and the first branch of the method, so the
     * registration can be pinned to the declaration rather than merely being
     * present somewhere.
     *
     * @return array<string, array{0: string, 1: string, 2: string}>
     */
    public static function conditionalProvider(): array
    {
        return [
            'if without else'        => ['conditional', 'x', 'if (flag)'],
            'two separate ifs'       => ['multiBranch', 'x', 'if (a)'],
            'loop that may not run'  => ['loopOnly', 'x', 'zephir_is_iterable('],
            'switch without default' => ['switchWithoutDefault', 'x', 'if (n == 1)'],
            'assignment inside try'  => ['tryOnly', 'x', 'try_start_1'],
            'string local'           => ['stringLocal', 's', 'if (flag)'],
        ];
    }

    /**
     * The mirror image: the local is definitely assigned at every read, so its
     * registration has to stay where the assignment put it.
     *
     * @return array<string, array{0: string, 1: string, 2: string}>
     */
    public static function definitelyAssignedProvider(): array
    {
        return [
            'unconditional write after a conditional one' => [
                'conditionalThenReassign',
                'x',
                'if (flag)',
            ],
            'switch with default' => [
                'switchWithDefault',
                'x',
                'if (n == 1)',
            ],
        ];
    }

    /**
     * @dataProvider conditionalProvider
     */
    public function testConditionallyAssignedLocalIsRegisteredAtItsDeclaration(
        string $method,
        string $name,
        string $firstBranch
    ): void {
        $body         = $this->methodBodyUntilNextMethod($this->compileFixture(), $method);
        $registration = 'ZEPHIR_INIT_VAR(&' . $name . ');';

        $this->assertStringContainsString(
            'ZVAL_UNDEF(&' . $name . ');',
            $body,
            sprintf(
                '%s() must keep "%s" undefined in the declaration block, which is what lets the '
                . 'registration below observe it.',
                $method,
                $name
            )
        );
        $this->assertSame(
            1,
            substr_count($body, $registration),
            sprintf(
                '%s() must register "%s" exactly once; a second observe of the same address '
                . 'aborts in a debug build.',
                $method,
                $name
            )
        );
        $this->assertLessThan(
            strpos($body, $firstBranch),
            strpos($body, $registration),
            sprintf(
                '%s() must register "%s" before "%s", or a path that skips the assignment still '
                . 'reads IS_UNDEF.',
                $method,
                $name,
                $firstBranch
            )
        );
    }

    /**
     * @dataProvider definitelyAssignedProvider
     */
    public function testDefinitelyAssignedLocalIsLeftAlone(
        string $method,
        string $name,
        string $firstBranch
    ): void {
        $body         = $this->methodBodyUntilNextMethod($this->compileFixture(), $method);
        $registration = 'ZEPHIR_INIT_VAR(&' . $name . ');';

        $this->assertGreaterThan(
            strpos($body, $firstBranch),
            strpos($body, $registration),
            sprintf(
                '%s() pays for a registration of "%s" it does not need: every one of them forces '
                . 'a memory frame on a method that may not have had one.',
                $method,
                $name
            )
        );
    }

    /**
     * An if/else that assigns in both arms already has its registration
     * hoisted out of the branch by the compiler, so position says nothing
     * here. What an added registration would leave behind is the downgrade of
     * that hoisted one to its N form.
     */
    public function testExhaustiveIfElseKeepsItsHoistedRegistration(): void
    {
        $body = $this->methodBodyUntilNextMethod($this->compileFixture(), 'exhaustiveIfElse');

        $this->assertSame(1, substr_count($body, 'ZEPHIR_INIT_VAR(&x);'));
        $this->assertStringNotContainsString(
            'ZEPHIR_INIT_NVAR(&x);',
            $body,
            'A second registration was added ahead of the one the if/else already hoists.'
        );
    }

    /**
     * `fetch` writes its target on both paths, the value on a hit and
     * ZVAL_NULL on a miss, so its target is already a valid zval. Touching it
     * would also be unsafe: a read-only fetch borrows its value without taking
     * a reference, so a registered slot would free a reference this method
     * never owned.
     */
    public function testFetchTargetIsNotRegistered(): void
    {
        $body = $this->methodBodyUntilNextMethod($this->compileFixture(), 'fetchCase');

        $this->assertStringContainsString(
            'zephir_memory_observe(&v);',
            $body,
            'The fetch target must keep its single unconditional observe.'
        );
        $this->assertStringNotContainsString(
            'ZEPHIR_INIT_VAR(&v);',
            $body,
            'Registering a fetch target ahead of the fetch adds a second observe.'
        );
    }

    /**
     * A local nothing ever writes to is #2654's case: the declaration itself
     * starts it at IS_NULL, which needs no memory frame at all.
     */
    public function testNeverAssignedLocalStillStartsAsNull(): void
    {
        $body = $this->methodBodyUntilNextMethod($this->compileFixture(), 'neverAssigned');

        $this->assertStringContainsString('ZVAL_NULL(&x);', $body);
        $this->assertStringNotContainsString('ZEPHIR_INIT_VAR(&x);', $body);
    }

    /**
     * The write is conditional but the read is not a zval read: a native local
     * has no IS_UNDEF to hand to userland and is not in the memory frame.
     */
    public function testNativeLocalIsUntouched(): void
    {
        $body = $this->methodBodyUntilNextMethod($this->compileFixture(), 'nativeLocal');

        $this->assertStringContainsString('zend_long i = 0;', $body);
        $this->assertStringNotContainsString('ZEPHIR_INIT_VAR(&i);', $body);
    }

    private function compileFixture(): string
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2679;

class Branch
{
    public function conditional(bool flag)
    {
        var x;

        if flag {
            let x = "set";
        }

        return x;
    }

    public function multiBranch(bool a, bool b)
    {
        var x;

        if a {
            let x = "a";
        }

        if b {
            let x = "b";
        }

        return x;
    }

    public function loopOnly(array items)
    {
        var x, item;

        for item in items {
            let x = item;
        }

        return x;
    }

    public function switchWithoutDefault(int n)
    {
        var x;

        switch n {
            case 1:
                let x = "one";
                break;
        }

        return x;
    }

    public function switchWithDefault(int n)
    {
        var x;

        switch n {
            case 1:
                let x = "one";
                break;

            default:
                let x = "other";
                break;
        }

        return x;
    }

    public function tryOnly(bool boom)
    {
        var x;

        try {
            if boom {
                throw new \RuntimeException("boom");
            }

            let x = "ok";
        } catch \RuntimeException {
            let boom = true;
        }

        return x;
    }

    public function stringLocal(bool flag)
    {
        string s;

        if flag {
            let s = "set";
        }

        return s;
    }

    public function exhaustiveIfElse(bool flag)
    {
        var x;

        if flag {
            let x = "yes";
        } else {
            let x = "no";
        }

        return x;
    }

    public function conditionalThenReassign(bool flag)
    {
        var x;

        if flag {
            let x = "first";
        }

        let x = "second";

        return x;
    }

    public function fetchCase(array data)
    {
        var v;
        bool found = false;

        if fetch v, data["k"] {
            let found = true;
        }

        return [found, v];
    }

    public function fetchGuarded(array data) -> string
    {
        var v;

        if fetch v, data["k"] {
            return (string) v;
        }

        return "missing";
    }

    public function neverAssigned()
    {
        var x;

        return x;
    }

    public function nativeLocal(bool flag) -> int
    {
        int i;

        if flag {
            let i = 7;
        }

        return i;
    }
}
ZEP;
        $this->compileSource('Stub\\Issue2679\\Branch', 'stub/issue2679/branch.zep', $zep);

        return $this->generatedC('stub/issue2679/branch.zep');
    }

    /**
     * Returns the C body of a single generated method.
     *
     * Not CompilesZephirSource::methodBody(): this one takes a bare method name
     * and runs to the next `PHP_METHOD(` marker rather than counting braces.
     */
    private function methodBodyUntilNextMethod(string $generated, string $method): string
    {
        $marker   = 'PHP_METHOD(Stub_Issue2679_Branch, ' . $method . ')';
        $startPos = strpos($generated, $marker);

        $this->assertNotFalse($startPos, 'Could not locate ' . $marker . ' in the generated C.');

        $endPos = strpos($generated, 'PHP_METHOD(', $startPos + strlen($marker));

        return false === $endPos
            ? substr($generated, $startPos)
            : substr($generated, $startPos, $endPos - $startPos);
    }
}

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

namespace Zephir\Test\Passes;

use PHPUnit\Framework\TestCase;
use Zephir\Parser\Parser;
use Zephir\Passes\NativeArrayPass;
use Zephir\StatementsBlock;

/**
 * A read-only subscript read borrows the value from its container, which only
 * a native array can back. This pass decides which locals are one.
 *
 * A wrong "proven" corrupts memory and a wrong "not proven" costs a refcount
 * pair, so most of what is below asserts the taint side.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class NativeArrayPassTest extends TestCase
{
    private string $tempFile;

    protected function setUp(): void
    {
        $this->tempFile = sys_get_temp_dir() . '/zephir_native_array_' . uniqid('', true) . '.zep';
    }

    protected function tearDown(): void
    {
        if (is_file($this->tempFile)) {
            unlink($this->tempFile);
        }
    }

    public function testAnArrayLiteralProves(): void
    {
        $this->assertProven('x', 'var x; let x = ["k": 1]; return x;');
    }

    public function testAnEmptyArrayLiteralProves(): void
    {
        $this->assertProven('x', 'var x; let x = []; let x[] = 1; return x;');
    }

    public function testAnArrayCastProves(): void
    {
        $this->assertProven('x', 'var x; let x = (array) data; return x;');
    }

    /**
     * The shape the benchmarks use: literals up front, then only array
     * updates, which cannot turn it into anything else.
     */
    public function testArrayUpdatesKeepTheProof(): void
    {
        $this->assertProven('x', '
            var x;
            let x = [];
            let x[0] = 1;
            let x[] = 2;
            let x["k"] = 3;
            return x;
        ');
    }

    public function testAnArrayParameterProves(): void
    {
        $this->assertProven('data', 'return data;');
    }

    public function testAnUntypedParameterDoesNotProve(): void
    {
        $this->assertNotProven('a', 'return a;');
    }

    /**
     * `array` is a compile-time label. Let\Variable::doArrayAssignmentProcess()
     * emits a bare ZEPHIR_CPY_WRT for this, so the object arrives intact.
     */
    public function testAnArrayLocalAliasedFromAVarDoesNotProve(): void
    {
        $this->assertNotProven('x', 'array x; let x = a; return x;');
    }

    /**
     * A declared `array` is proof on its own, but only because every way of
     * putting something else in it taints.
     */
    public function testADeclaredArrayLocalProvesUntilSomethingElseIsWrittenToIt(): void
    {
        $this->assertProven('x', 'array x; let x[] = 1; return x;');
        $this->assertNotProven('x', 'array x; let x = someCall(); return x;');
    }

    public function testAssignmentFromAnythingElseTaints(): void
    {
        $this->assertNotProven('x', 'var x; let x = []; let x = a; return x;');
        $this->assertNotProven('x', 'var x; let x = []; let x = someCall(); return x;');
        $this->assertNotProven('x', 'var x; let x = []; let x = this->prop; return x;');
        $this->assertNotProven('x', 'var x; let x = []; let x = 1; return x;');
        $this->assertNotProven('x', 'var x; let x = []; let x = null; return x;');
    }

    /**
     * The order does not matter: the pass sees the whole method, so a loop
     * that reassigns the container after the read cannot slip past.
     */
    public function testAWriteAfterTheReadStillTaints(): void
    {
        $this->assertNotProven('x', '
            var x, y;
            let x = [];
            while a {
                let y = x[0];
                let x = someCall();
            }
            return y;
        ');
    }

    public function testACompoundAssignmentTaints(): void
    {
        $this->assertNotProven('x', 'var x; let x = []; let x += 1; return x;');
    }

    public function testAnIncrementTaints(): void
    {
        $this->assertNotProven('x', 'var x; let x = []; let x++; return x;');
    }

    /**
     * A by-reference parameter of the callee writes straight into the caller's
     * zval, and which parameters those are is unknown here.
     */
    public function testACallArgumentTaints(): void
    {
        $this->assertNotProven('x', 'var x; let x = []; someCall(x); return x;');
        $this->assertNotProven('x', 'var x; let x = []; this->method(x); return x;');
        $this->assertNotProven('x', 'var x; let x = []; Other::method(x); return x;');
    }

    public function testAFetchTargetTaints(): void
    {
        $this->assertNotProven('x', 'var x; let x = []; if fetch x, data["k"] { return x; } return x;');
    }

    public function testALoopSlotTaints(): void
    {
        $this->assertNotProven('v', 'var k, v; let v = []; for k, v in data { return v; } return v;');
        $this->assertNotProven('k', 'var k, v; let k = []; for k, v in data { return k; } return k;');
    }

    public function testACatchVariableTaints(): void
    {
        $this->assertNotProven('e', '
            var e;
            let e = [];
            try {
                return 1;
            } catch \Exception, e {
                return e;
            }
        ');
    }

    public function testADestructuringSlotTaints(): void
    {
        $this->assertNotProven('x', 'var x, y; let x = []; let [x, y] = data; return x;');
    }

    public function testUnsettingTaints(): void
    {
        $this->assertNotProven('x', 'var x; let x = []; unset(x); return x;');
    }

    /**
     * Removing a key leaves an array behind, so it says nothing.
     */
    public function testUnsettingAnIndexDoesNotTaint(): void
    {
        $this->assertProven('x', 'var x; let x = []; unset(x["k"]); return x;');
    }

    /**
     * A by-reference capture shares one slot with the closure. A by-value one
     * does not, but an arrow closure's list is implicit, so both taint.
     */
    public function testAClosureCaptureTaints(): void
    {
        $this->assertNotProven('x', '
            var x, f;
            let x = [];
            let f = function() use (&x) { return x; };
            return f;
        ');
    }

    /**
     * A closure body is a scope of its own, so what it does to its own locals
     * says nothing about this method's.
     */
    public function testAClosureBodyIsNotThisScope(): void
    {
        $this->assertProven('x', '
            var x, f;
            let x = [];
            let f = function() { var x; let x = someCall(); return x; };
            return x[0];
        ');
    }

    private function analyse(string $body): NativeArrayPass
    {
        $source = "namespace Test;\n\nclass Probe\n{\n"
            . "    public function run(bool a, var b, array data)\n    {\n"
            . $body
            . "\n    }\n}\n";

        file_put_contents($this->tempFile, $source);

        $ir = (new Parser())->parse($this->tempFile);

        $method = null;
        foreach ($ir as $node) {
            if ('class' === $node['type']) {
                $method = $node['definition']['methods'][0];
                break;
            }
        }

        $this->assertNotNull($method, 'The probe did not parse into a method body.');

        $pass = new NativeArrayPass();
        $pass->passParameters($method['parameters']);
        $pass->pass(new StatementsBlock($method['statements']));

        return $pass;
    }

    private function assertNotProven(string $name, string $body): void
    {
        $this->assertFalse(
            $this->analyse($body)->isProvenNativeArray($name),
            sprintf('"%s" can hold something other than an array, so a read of it must not borrow.', $name)
        );
    }

    private function assertProven(string $name, string $body): void
    {
        $this->assertTrue(
            $this->analyse($body)->isProvenNativeArray($name),
            sprintf('"%s" can only hold an array, so a read of it may borrow.', $name)
        );
    }
}

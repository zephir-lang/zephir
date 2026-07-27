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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use ReflectionClass;
use Zephir\FunctionCall;

/**
 * Covers optimizer lookup, which the `missing-optimizer` warning depends on to
 * tell "no optimizer exists" from "an optimizer exists but declined this call".
 *
 * @see https://github.com/zephir-lang/zephir/issues/2468
 */
final class FunctionCallTest extends TestCase
{
    private FunctionCall $functionCall;

    public function testAcosOptimizerIsReachable(): void
    {
        $this->registerBuiltInOptimizerDir();

        // Regression lock: the file used to be named ACosOptimizer.php, which
        // Name::camelize('acos') can never produce, so it never loaded on a
        // case-sensitive filesystem.
        $this->assertTrue($this->functionCall->hasOptimizer('acos'));
        $this->assertTrue($this->functionCall->hasOptimizer('asin'));
    }

    public function testLookupBeforeDirectoriesAreRegisteredDoesNotPoisonTheCache(): void
    {
        // Optimizer directories are only registered inside Compiler::generate(),
        // so a lookup can legitimately happen before any directory is known.
        $this->assertFalse($this->functionCall->hasOptimizer('strlen'));

        $this->registerBuiltInOptimizerDir();

        $this->assertTrue(
            $this->functionCall->hasOptimizer('strlen'),
            'A lookup made before any optimizer directory was registered must not '
            . 'cache a negative result for the rest of the process.'
        );
    }

    public function testReportsNoOptimizerForUnoptimizedFunction(): void
    {
        $this->registerBuiltInOptimizerDir();

        $this->assertFalse($this->functionCall->hasOptimizer('array_product'));
    }

    public function testResolvesOptimizerForOptimizedFunction(): void
    {
        $this->registerBuiltInOptimizerDir();

        $this->assertTrue($this->functionCall->hasOptimizer('strlen'));
        $this->assertTrue($this->functionCall->hasOptimizer('str_replace'));
    }

    protected function setUp(): void
    {
        $this->resetOptimizerState();
        $this->functionCall = new FunctionCall();
    }

    protected function tearDown(): void
    {
        $this->resetOptimizerState();
    }

    private function registerBuiltInOptimizerDir(): void
    {
        FunctionCall::addOptimizerDir(\constant('ZEPHIRPATH') . 'src/Optimizers/FunctionCall');
    }

    /**
     * Optimizer directories and the resolved-optimizer cache are process-wide
     * statics; clear them so tests cannot leak into each other.
     */
    private function resetOptimizerState(): void
    {
        $reflection = new ReflectionClass(FunctionCall::class);

        foreach (['optimizerDirectories', 'optimizers'] as $name) {
            $property = $reflection->getProperty($name);
            $property->setAccessible(true);
            $property->setValue(null, []);
        }
    }
}

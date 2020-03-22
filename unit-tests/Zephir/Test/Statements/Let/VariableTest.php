<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Statements\Let\Variable;
use Zephir\Variable as ZephirVariable;

class VariableTest extends TestCase
{
    /** @var Variable */
    private $test;

    public function setUp()
    {
        $this->test = new Variable();
    }

    /** @test */
    public function cannotMutateReadonlyVariable()
    {
        $zephirVariable = new ZephirVariable('variable', 'foo');
        $zephirVariable->setReadOnly(true);

        $compiledExpr = new CompiledExpression('variable', '', []);

        $this->expectException(CompilerException::class);
        $this->expectExceptionCode(0);
        $this->expectExceptionMessage('Cannot mutate variable \'foo\' because it is read only');

        $this->test->assign('foo', $zephirVariable, $compiledExpr, new ReadDetector(), new CompilationContext(), []);
    }

    /** @test */
    public function cannotMutateNotInitializedVariable()
    {
        $this->expectException(CompilerException::class);
        $this->expectExceptionMessage('Cannot mutate variable \'foo\' because it is not initialized');

        $zephirVariable = new ZephirVariable('variable', 'foo');

        $compiledExpr = new CompiledExpression('variable', '', []);

        $this->test->assign('foo', $zephirVariable, $compiledExpr, new ReadDetector(), new CompilationContext(), ['operator' => 'concat']);
    }
}

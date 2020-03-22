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

class VariableTest extends TestCase
{
    /** @test */
    public function cannotMutateReadonlyVariable()
    {
        $test = new Variable();

        $zephirVariable = new \Zephir\Variable('variable', 'foo');
        $zephirVariable->setReadOnly(true);

        $compiledExpr = new CompiledExpression('variable', '', []);

        $this->expectException(CompilerException::class);
        $this->expectExceptionCode(0);
        $this->expectExceptionMessage('Cannot mutate variable \'foo\' because it is read only');

        $test->assign('foo', $zephirVariable, $compiledExpr, new ReadDetector(), new CompilationContext(), []);
    }
}

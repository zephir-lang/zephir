<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\Statements\Let;

use PHPUnit\Framework\TestCase;
use Zephir\BranchManager;
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

    /** @var ReadDetector */
    private $readDetector;

    /** @var CompilationContext */
    private $compilationCtx;

    /** @var CompiledExpression */
    private $compiledExpr;

    public function setUp()
    {
        $this->test = new Variable();
        $this->readDetector = new ReadDetector();
        $this->compiledExpr = new CompiledExpression('variable', '', []);
        $this->compilationCtx = new CompilationContext();
        $this->compilationCtx->branchManager = new BranchManager();
    }

    public function exceptionTestProvider(): array
    {
        return [
            'immutable variable' => [
                [],
                'Cannot mutate variable \'foo\' because it is read only',
                function (ZephirVariable $var) {
                    $var->setReadOnly(true);
                },
            ],
            'not initialized variable' => [
                ['operator' => 'concat'],
                'Cannot mutate variable \'foo\' because it is not initialized',
                function (ZephirVariable $var) {
                    return;
                },
            ],
            'unknown variable type' => [
                ['operator' => 'assign'],
                'Unknown type: prototype',
                function (ZephirVariable $var) {
                    $var->setType('prototype');
                },
            ],
        ];
    }

    /**
     * @test
     * @dataProvider exceptionTestProvider
     *
     * @param array    $statement
     * @param string   $message     - Expected error message
     * @param callable $setProperty - Callback func to set required for test property
     */
    public function throwsExceptionForWrongVariables(array $statement, string $message, callable $setProperty)
    {
        $zephirVariable = new ZephirVariable('variable', 'foo');
        // call necessary setters for test case
        $setProperty($zephirVariable);

        $this->expectException(CompilerException::class);
        $this->expectExceptionCode(0);
        $this->expectExceptionMessage($message);

        $this->test->assign('foo', $zephirVariable, $this->compiledExpr, $this->readDetector, $this->compilationCtx, $statement);
    }
}

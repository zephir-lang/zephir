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

namespace Zephir\Test\Statements\Let;

use PHPUnit\Framework\TestCase;
use Zephir\BranchManager;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Statements\Let\Variable;
use Zephir\Variable\Variable as ZephirVariable;

final class VariableTest extends TestCase
{
    private Variable $test;
    private ReadDetector $readDetector;
    private CompilationContext $compilationCtx;
    private CompiledExpression $compiledExpr;

    protected function setUp(): void
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
                function (ZephirVariable $var): void {
                    $var->setReadOnly(true);
                },
            ],
            'not initialized variable' => [
                ['operator' => 'concat'],
                'Cannot mutate variable \'foo\' because it is not initialized',
                fn (ZephirVariable $var) => null,
            ],
            'unknown variable type' => [
                ['operator' => 'assign'],
                'Unknown type: prototype',
                function (ZephirVariable $var): void {
                    $var->setType('prototype');
                },
            ],
        ];
    }

    /**
     * @dataProvider exceptionTestProvider
     *
     * @param array    $statement
     * @param string   $message     - Expected error message
     * @param callable $setProperty - Callback func to set required for test property
     */
    public function testThrowsExceptionForWrongVariables(array $statement, string $message, callable $setProperty): void
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

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

namespace Zephir\Statements;

use Zephir\Class\Definition\GeneratorTransformer;
use Zephir\Class\Method\Method;
use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;

/**
 * Suspension point of a generator step method (issue #1849).
 *
 * A generator method's body compiles inside a hidden internal "step"
 * function (see GeneratorTransformer). Each `yield` publishes the key/value
 * pair on the <Ns>\Generator object, saves every suspendable local into the
 * object's slots, releases the memory frame and returns; the resume label
 * right after it is the target of the step's dispatch switch.
 *
 * Grammar forms: `yield;` (NULL value, auto key), `yield expr;` (auto key),
 * `yield key, value;`.
 */
final class YieldStatement extends StatementAbstract
{
    /**
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $method = $compilationContext->currentMethod;
        if (!$method instanceof Method || !$method->isGeneratorStep()) {
            throw new CompilerException(
                "'yield' is only supported inside class or trait methods; functions and closures "
                . 'cannot be generators yet. See https://github.com/zephir-lang/zephir/issues/1849',
                $this->statement
            );
        }

        if ($compilationContext->insideTryCatch > 0) {
            throw new CompilerException(
                "'yield' inside a try/catch block is not supported yet. "
                . 'See https://github.com/zephir-lang/zephir/issues/1849',
                $this->statement
            );
        }

        $compilationContext->headersManager->add('kernel/generator');

        $codePrinter = $compilationContext->codePrinter;
        $genVariable = $compilationContext->symbolTable->getVariableForRead(
            GeneratorTransformer::GEN_PARAM,
            $compilationContext,
            $this->statement
        );
        $gen = $compilationContext->backend->getVariableCode($genVariable);

        $keyCode = isset($this->statement['key'])
            ? GeneratorZvalResolver::resolve($this->statement['key'], $compilationContext)
            : 'NULL';

        $valueAst  = $this->statement['value'] ?? $this->statement['expr'] ?? null;
        $valueCode = null !== $valueAst
            ? GeneratorZvalResolver::resolve($valueAst, $compilationContext)
            : 'NULL';

        $state = $method->allocateYieldPoint();

        $codePrinter->output(
            'zephir_generator_yield(' . $gen . ', ' . $keyCode . ', ' . $valueCode . ', ' . $state . ');'
        );
        /* Expanded post-compile with the per-slot save sequence (Method::generatorSaveCode). */
        $codePrinter->output('//%ZEPHIR_GEN_SAVE%');
        $codePrinter->output('ZEPHIR_MM_RESTORE();');
        $codePrinter->output('return;');
        $codePrinter->output('zephir_yield_resume_' . $state . ':;');
    }
}

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

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;

use function in_array;

/**
 * unset() statement
 */
class UnsetStatement extends StatementAbstract
{
    /**
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        $flags = 'PH_SEPARATE';

        $compilationContext->headersManager->add('kernel/array');
        $expression = $this->statement['expr'];

        if ('list' === $expression['type']) {
            $expression = $expression['left'];
        }

        switch ($expression['type']) {
            case 'array-access':
                /**
                 * Unset of object property
                 */
                if (isset($expression['left']['type']) && $expression['left']['type'] === 'property-access') {
                    $compilationContext = $this->generateUnsetPropertyFromObject($expression, $compilationContext);
                }

                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar  = $expr->compile($compilationContext);
                $variable = $compilationContext->symbolTable->getVariableForWrite(
                    $exprVar->getCode(),
                    $compilationContext,
                    $this->statement
                );

                $expr = new Expression($expression['right']);
                $expr->setReadOnly(true);
                $exprIndex = $expr->compile($compilationContext);
                break;

            case 'property-access':
            case 'property-dinamic-access':
                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar      = $expr->compile($compilationContext);
                $variable     = $compilationContext->symbolTable->getVariableForWrite(
                    $exprVar->getCode(),
                    $compilationContext,
                    $this->statement
                );
                $variableCode = $compilationContext->backend->getVariableCode($variable);

                $compilationContext->headersManager->add('kernel/object');
                $compilationContext->codePrinter->output(
                    'zephir_unset_property(' . $variableCode . ', "' . $expression['right']['value'] . '");'
                );

                return;

            default:
                throw new CompilerException(
                    'Cannot use expression type: ' . $expression['type'] . ' in "unset"',
                    $expression
                );
        }

        if (!in_array($variable->getType(), ['variable', 'array'])) {
            throw CompilerException::cannotUseVariableTypeAs(
                $variable,
                'in "unset"',
                $expression['left']
            );
        }

        if ($variable->hasDifferentDynamicType(['undefined', 'array', 'object', 'null'])) {
            $compilationContext->logger->warning(
                'Possible attempt to use non array/object in unset operator',
                ['non-valid-unset', $expression['left']]
            );
        }

        $compilationContext->backend->arrayUnset($variable, $exprIndex, $flags, $compilationContext);
    }

    /**
     * @throws Exception
     * @throws ReflectionException
     */
    private function generateUnsetPropertyFromObject(
        array $expression,
        CompilationContext $compilationContext
    ): CompilationContext {
        $expr = new Expression($expression['right']);
        $expr->setReadOnly(true);
        $exprVar = $expr->compile($compilationContext);

        switch ($exprVar->getType()) {
            case 'variable':
                $variable    = $compilationContext->symbolTable->getVariableForRead(
                    $exprVar->getCode(),
                    $compilationContext,
                    $this->statement
                );
                $variableRef = $compilationContext->backend->getVariableCode($variable);
                break;

            default:
                $expr = new Expression($expression['left']);
                $expr->setReadOnly(true);
                $exprVar  = $expr->compile($compilationContext);
                $variable = $compilationContext->symbolTable->getVariableForWrite(
                    $exprVar->getCode(),
                    $compilationContext,
                    $this->statement
                );

                $variableRef = $compilationContext->backend->getVariableCode($variable);

                // TODO: Add more types check when parser will support them, see ArrayAccessTest.zep
                switch ($expression['right']['type']) {
                    case 'string':
                        $compilationContext->codePrinter->output(
                            'ZVAL_STR(' . $variableRef . ', "' . $expression['right']['value'] . '");'
                        );
                        break;

                    case 'int':
                        $compilationContext->codePrinter->output(
                            'ZVAL_LONG(' . $variableRef . ', ' . $expression['right']['value'] . ');'
                        );
                        break;
                }
                break;
        }

        $expr = new Expression($expression['left']['left']);
        $expr->setReadOnly(true);
        $exprVar      = $expr->compile($compilationContext);
        $variable     = $compilationContext->symbolTable->getVariableForWrite(
            $exprVar->getCode(),
            $compilationContext,
            $this->statement
        );
        $variableCode = $compilationContext->backend->getVariableCode($variable);

        $compilationContext->headersManager->add('kernel/object');
        $compilationContext->codePrinter->output(
            'zephir_unset_property_array(' . $variableCode . ', ZEND_STRL("' . $expression['left']['right']['value'] . '"), ' . $variableRef . ');'
        );

        return $compilationContext;
    }
}

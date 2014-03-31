<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Builder\FunctionCallBuilder;

/**
 * Cast
 *
 * Converts a value into another of a different type
 */
class CastOperator extends BaseOperator
{
    /**
     * @param $expression
     * @param CompilationContext $compilationContext
     * @return bool|CompiledExpression
     * @throws CompilerException
     */
    public function compile($expression, CompilationContext $compilationContext)
    {

        $expr = new Expression($expression['right']);
        $resolved = $expr->compile($compilationContext);

        switch ($expression['left']) {

            case 'int':
                switch ($resolved->getType()) {

                    case 'int':
                        return new CompiledExpression('int', $resolved->getCode(), $expression);

                    case 'double':
                        return new CompiledExpression('int', '(int) ' . $resolved->getCode(), $expression);

                    case 'bool':
                        return new CompiledExpression('int', $resolved->getBooleanCode(), $expression);

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        switch ($symbolVariable->getType()) {
                            case 'int':
                                return new CompiledExpression('int', $symbolVariable->getName(), $expression);
                            case 'double':
                                return new CompiledExpression('int', '(int) (' . $symbolVariable->getName() . ')', $expression);
                            case 'variable':
                                return new CompiledExpression('int', 'zephir_get_intval(' . $symbolVariable->getName() . ')', $expression);
                            default:
                                throw new CompilerException("Cannot cast: " . $resolved->getType() . "(" . $symbolVariable->getType() . ") to " . $expression['left'], $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'long':
                switch ($resolved->getType()) {

                    case 'int':
                        return new CompiledExpression('long', $resolved->getCode(), $expression);

                    case 'double':
                        return new CompiledExpression('long', '(long) ' . $resolved->getCode(), $expression);

                    case 'bool':
                        return new CompiledExpression('long', $resolved->getBooleanCode(), $expression);

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        switch ($symbolVariable->getType()) {
                            case 'int':
                                return new CompiledExpression('long', $symbolVariable->getName(), $expression);
                            case 'double':
                                return new CompiledExpression('long', '(long) (' . $symbolVariable->getName() . ')', $expression);
                            case 'variable':
                                return new CompiledExpression('long', 'zephir_get_intval(' . $symbolVariable->getName() . ')', $expression);
                            default:
                                throw new CompilerException("Cannot cast: " . $resolved->getType() . "(" . $symbolVariable->getType() . ") to " . $expression['left'], $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'double':
                switch ($resolved->getType()) {

                    case 'double':
                        return new CompiledExpression('int', $resolved->getCode(), $expression);

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        return new CompiledExpression('double', 'zephir_get_doubleval(' . $symbolVariable->getName() . ')', $expression);

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'bool':
                switch ($resolved->getType()) {
                    case 'bool':
                        return new CompiledExpression('bool', $resolved->getCode(), $expression);

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }
                        switch ($symbolVariable->getType()) {
                            case 'variable':
                                return new CompiledExpression('bool', 'zephir_get_boolval(' . $symbolVariable->getName() . ')', $expression);
                            default:
                                throw new CompilerException("Cannot cast: " . $resolved->getType() . "(" . $symbolVariable->getType() . ") to " . $expression['left'], $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'string':
                switch ($resolved->getType()) {
                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getTempVariable('string', $compilationContext, $expression);
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->setIsInitialized(true, $compilationContext, $expression);
                        $compilationContext->codePrinter->output('zephir_get_strval(' . $symbolVariable->getName() . ', ' . $resolved->getCode() . ');');
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }
                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'char':
                switch ($resolved->getType()) {
                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext, $expression);
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        $compilationContext->codePrinter->output($tempVariable->getName() . ' = (char) zephir_get_intval(' . $symbolVariable->getName() . ');');
                        return new CompiledExpression('variable', $tempVariable->getName(), $expression);
                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            default:
                throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
        }
    }
}

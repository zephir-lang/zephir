<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

use Zephir\Detectors\ReadDetector;
use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Statements\Let\Variable as LetVariable;
use Zephir\Builder\FunctionCallBuilder;

/**
 * Cast
 *
 * Converts a value into another of a different type
 */
class CastOperator extends BaseOperator
{
    /**
     * Compiles a type cast operation
     *
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

                    case 'null':
                        return new CompiledExpression('int', 0, $expression);

                    case 'int':
                        return new CompiledExpression('int', $resolved->getCode(), $expression);

                    case 'double':
                        return new CompiledExpression('int', '(int) ' . $resolved->getCode(), $expression);

                    case 'bool':
                        return new CompiledExpression('int', $resolved->getBooleanCode(), $expression);

                    case 'string':
                        $compilationContext->headersManager->add('kernel/operators');

                        /**
                         * zephir_get_intval_ex use zval variable
                         * before use with it we create a new variable and assign value of literal
                         *
                         * @todo Optimize by creating native function for string without zval using
                         */
                        $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('string', $compilationContext);
                        $let = new LetVariable();
                        $original = $resolved->getOriginal();
                        $original['operator'] = 'assign';
                        $let->assign($symbolVariable->getName(), $symbolVariable, $resolved, new ReadDetector(), $compilationContext, $original);

                        return new CompiledExpression('int', 'zephir_get_intval_ex(' . $symbolVariable->getName() . ')', $expression);

                    case 'array':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        return new CompiledExpression('int', 'zephir_get_intval(' . $symbolVariable->getName() . ')', $expression);

                    case 'variable':

                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        switch ($symbolVariable->getType()) {
                            case 'int':
                                return new CompiledExpression('int', $symbolVariable->getName(), $expression);

                            case 'double':
                                return new CompiledExpression('int', '(int) (' . $symbolVariable->getName() . ')', $expression);

                            case 'bool':
                                return new CompiledExpression('int', '(int) (' . $symbolVariable->getName() . ')', $expression);

                            case 'array':
                            case 'variable':
                            case 'string':
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

                    case 'array':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        return new CompiledExpression('long', 'zephir_get_intval(' . $symbolVariable->getName() . ')', $expression);

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
                    case 'null':
                        return new CompiledExpression('double', 0, $expression);

                    case 'bool':
                        return new CompiledExpression('double', $resolved->getBooleanCode(), $expression);

                    case 'double':
                        return new CompiledExpression('double', $resolved->getCode(), $expression);

                    case 'array':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        return new CompiledExpression('double', 'zephir_get_doubleval(' . $symbolVariable->getName() . ')', $expression);

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);

                        switch ($symbolVariable->getType()) {
                            case 'int':
                                return new CompiledExpression('double', $symbolVariable->getName(), $expression);
                            case 'double':
                                return new CompiledExpression('double', '(double) (' . $symbolVariable->getName() . ')', $expression);
                            case 'bool':
                                return new CompiledExpression('double', '(double) (' . $symbolVariable->getName() . ')', $expression);
                            case 'array':
                            case 'variable':
                                return new CompiledExpression('double', 'zephir_get_doubleval(' . $symbolVariable->getName() . ')', $expression);
                            default:
                                throw new CompilerException("Cannot cast: " . $resolved->getType() . "(" . $symbolVariable->getType() . ") to " . $expression['left'], $expression);
                        }
                        break;

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'bool':
                switch ($resolved->getType()) {
                    case 'int':
                        return new CompiledExpression('bool', '(zend_bool) '.$resolved->getCode(), $expression);
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

            case 'char':
                switch ($resolved->getType()) {

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext);
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        $compilationContext->codePrinter->output($tempVariable->getName() . ' = (char) zephir_get_intval(' . $symbolVariable->getName() . ');');

                        return new CompiledExpression('variable', $tempVariable->getName(), $expression);

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'string':
                switch ($resolved->getType()) {

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getTempVariable('string', $compilationContext);
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->setIsInitialized(true, $compilationContext);
                        $symbolVariable->increaseUses();
                        $compilationContext->codePrinter->output('zephir_get_strval(' . $symbolVariable->getName() . ', ' . $resolved->getCode() . ');');
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }
                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'array':
                switch ($resolved->getType()) {

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getTempVariable('array', $compilationContext, $expression);
                        $symbolVariable->setMustInitNull(true);
                        $symbolVariable->setIsInitialized(true, $compilationContext, $expression);
                        $symbolVariable->increaseUses();
                        $compilationContext->codePrinter->output('zephir_get_arrval(' . $symbolVariable->getName() . ', ' . $resolved->getCode() . ');');
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }
                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            case 'object':

                switch ($resolved->getType()) {
                    case 'int':
                    case 'double':
                    case 'bool':
                    case 'null':
                    case 'string':
                    case 'array':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getTempVariable('variable', $compilationContext);

                        /**
                         * zephir_convert_to_object use zval variable
                         * before use with it we create a new variable and assign value of literal
                         */
                        $let = new LetVariable();
                        $original = $resolved->getOriginal();
                        $original['operator'] = 'assign';
                        $let->assign($symbolVariable->getName(), $symbolVariable, $resolved, new ReadDetector(), $compilationContext, $original);

                        $compilationContext->codePrinter->output('zephir_convert_to_object('.$symbolVariable->getName().');');
                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);

                    case 'variable':
                        $compilationContext->headersManager->add('kernel/operators');
                        $symbolVariable = $compilationContext->symbolTable->getVariableForRead($resolved->getCode(), $compilationContext, $expression);
                        if ($symbolVariable->isTemporal()) {
                            $symbolVariable->setIdle(true);
                        }

                        $compilationContext->codePrinter->output('zephir_convert_to_object(' . $symbolVariable->getName() . ');');
                        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);

                    default:
                        throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
                }
                break;

            default:
                throw new CompilerException("Cannot cast: " . $resolved->getType() . " to " . $expression['left'], $expression);
        }
    }
}

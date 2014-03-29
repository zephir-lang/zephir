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

namespace Zephir\Statements\Let;

use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\Variable as ZephirVariable;
use Zephir\Detectors\ReadDetector;
use Zephir\Expression;
use Zephir\CompiledExpression;
use Zephir\Compiler;
use Zephir\Utils;
use Zephir\GlobalConstant;

/**
 * ObjectPropertyAppend
 *
 * Appends a value to a property
 */
class ObjectPropertyAppend
{

	/**
     * Compiles x->y[] = foo
     *
     * @param string $variable
     * @param Variable $symbolVariable
     * @param CompiledExpression $resolvedExpr
     * @param CompilationContext $compilationContext,
     * @param array $statement
     */
    public function assign($variable, ZephirVariable $symbolVariable, CompiledExpression $resolvedExpr, CompilationContext $compilationContext, $statement)
    {

        if (!$symbolVariable->isInitialized()) {
            throw new CompilerException("Cannot mutate variable '" . $variable . "' because it is not initialized", $statement);
        }

        if ($symbolVariable->getType() != 'variable') {
            throw new CompilerException("Attempt to use variable type: " . $symbolVariable->getType() . " as object", $statement);
        }

        $codePrinter = $compilationContext->codePrinter;

        $property = $statement['property'];
        $compilationContext->headersManager->add('kernel/object');

        /* @todo check whether property really does exist */

        switch ($resolvedExpr->getType()) {
            case 'variable':
                $variableExpr = $compilationContext->symbolTable->getVariableForRead($resolvedExpr->getCode(), $compilationContext, $statement);
                switch ($variableExpr->getType()) {
                    case 'variable':
                    case 'string':
                    case 'array':
                        $codePrinter->output('zephir_update_property_array_append(' . $symbolVariable->getName() . ', SL("' . $property . '"), ' . $variableExpr->getName() . ' TSRMLS_CC);');
                        if ($variableExpr->isTemporal()) {
                            $variableExpr->setIdle(true);
                        }
                        break;
                    default:
                        throw new CompilerException("Variable: " . $variableExpr->getType() . " cannot be appended to array property", $statement);
                }
                break;
            default:
                throw new CompilerException("Expression: " . $resolvedExpr->getType() . " cannot be appended to array property", $statement);
        }
    }
}
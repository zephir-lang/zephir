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

namespace Zephir\Operators\Logical;

use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Statements\LetStatement;
use Zephir\CompilerException;
use Zephir\CompiledExpression;

class OrOperator extends LogicalBaseOperator
{

    protected $_operator = '||';

    protected $_bitOperator = '||';

    public function compile($expression, CompilationContext $compilationContext)
    {
        if (!isset($expression['left'])) {
            throw new \Exception("Missing left part of the expression");
        }

        if (!isset($expression['right'])) {
            throw new \Exception("Missing right part of the expression");
        }

        $leftExpr = new Expression($expression['left']);
        $leftExpr->setReadOnly($this->_readOnly);
        $left = $leftExpr->compile($compilationContext);

        /**
         * This variable is used to check if the compound and expression is evaluated as true or false
         */
        $flagVariable = $compilationContext->symbolTable->getTempVariableForWrite('bool', $compilationContext);

        switch ($left->getType()) {
            case 'int':
            case 'bool':
            case 'char':
            case 'double':
            case 'uint':
            case 'uchar':
                $assignExprLeft = array(
                    'type'  => $left->getType(),
                    'value' => $left->getCode(),
                );
                break;
            case 'variable':
                $assignExprLeft = array(
                    'type'  => 'variable',
                    'value' => $left->getCode(),
                );
                break;
            case 'null':
                $assignExprLeft = array(
                    'type'  => 'null',
                    'value' => null
                );
                break;
        }

        if (!isset($assignExprLeft)) {
            throw new CompilerException($left->getType(), $expression['left']);
        }

        /**
         * Create an implicit 'let' operation to update the evaluated left operator
         */
        $statement = new LetStatement(array(
            'type' => 'let',
            'assignments' => array(
                array(
                    'assign-type' => 'variable',
                    'variable' => $flagVariable->getName(),
                    'operator' => 'assign',
                    'expr' => $assignExprLeft,
                    'file' => $expression['left']['file'],
                    'line' => $expression['left']['line'],
                    'char' => $expression['left']['char'],
                )
            )
        ));
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->output('if (!(' . $flagVariable->getName() . ')) {');

        $compilationContext->codePrinter->increaseLevel();

        $rightExpr = new Expression($expression['right']);
        $rightExpr->setReadOnly($this->_readOnly);
        $right = $rightExpr->compile($compilationContext);

        switch ($right->getType()) {
            case 'int':
            case 'bool':
            case 'char':
            case 'double':
            case 'uint':
            case 'uchar':
                $assignExprRight = array(
                    'type'  => $right->getType(),
                    'value' => $right->getCode(),
                );
                break;
            case 'variable':
                $assignExprRight = array(
                    'type'  => 'variable',
                    'value' => $right->getCode(),
                );
                break;
            case 'null':
                $assignExprRight = array(
                    'type'  => 'null',
                    'value' => null
                );
                break;
        }

        if (!isset($assignExprRight)) {
            throw new CompilerException($right->getType(), $expression['right']);
        }

        /**
         * Create an implicit 'let' operation to update the evaluated right operator
         */
        $statement = new LetStatement(array(
            'type' => 'let',
            'assignments' => array(
                array(
                    'assign-type' => 'variable',
                    'variable' => $flagVariable->getName(),
                    'operator' => 'assign',
                    'expr' => $assignExprRight,
                    'file' => $expression['right']['file'],
                    'line' => $expression['right']['line'],
                    'char' => $expression['right']['char'],
                )
            )
        ));
        $statement->compile($compilationContext);

        $compilationContext->codePrinter->decreaseLevel();

        $compilationContext->codePrinter->output('}');

        return new CompiledExpression('bool', $flagVariable->getName(), $expression);
    }
}

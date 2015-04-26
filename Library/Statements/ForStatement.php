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

namespace Zephir\Statements;

use Zephir\Compiler;
use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\FunctionCall;
use Zephir\Optimizers\EvalExpression;
use Zephir\StatementsBlock;
use Zephir\Expression;
use Zephir\Detectors\ForValueUseDetector;
use Zephir\Variable;
use Zephir\Utils;

/**
 * ForStatement
 *
 * For statement
 */
class ForStatement extends StatementAbstract
{
    /**
     * Compiles a for statement that use a 'range' as expression
     *
     * @param array $exprRaw
     * @param \CompilationContext $compilationContext
     * @return boolean
     */
    public function compileRange($exprRaw, CompilationContext $compilationContext)
    {
        if (!count($exprRaw['parameters'])) {
            return false;
        }

        if (count($exprRaw['parameters']) > 3) {
            return false;
        }

        $functionCall = new FunctionCall();
        $parameters = $functionCall->getResolvedParamsAsExpr($exprRaw['parameters'], $compilationContext, $exprRaw);

        if (count($parameters) != 2 && count($parameters) != 3) {
            throw new CompilerException("Wrong number of parameters", $this->_statement['expr']);
        }

        if ($parameters[0]->getType() != 'variable') {
            if (!$parameters[0]->isIntCompatibleType()) {
                return false;
            }
        }

        if ($parameters[1]->getType() != 'variable') {
            if (!$parameters[1]->isIntCompatibleType()) {
                return false;
            }
        }

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Initialize 'key' variable
         */
        if (isset($this->_statement['key'])) {
            /**
             * This variable is used to check if the loop is in its first iteration
             */
            $keyVariable = $compilationContext->symbolTable->getTempVariableForWrite('long', $compilationContext, $this->_statement);
            $keyVariable->increaseUses();
        }

        /**
         * This variable is used to check if the loop is in its first iteration
         */
        $flagVariable = $compilationContext->symbolTable->getTempVariableForWrite('bool', $compilationContext, $this->_statement);

        if ($parameters[0]->getType() != 'variable') {
            $tempVariable = $compilationContext->symbolTable->addTemp($parameters[0]->getType(), $compilationContext);
        } else {
            $rangeVariable = $compilationContext->symbolTable->getVariableForRead($parameters[0]->getCode(), $compilationContext, $this->_statement['expr']);
            $tempVariable = $compilationContext->symbolTable->addTemp($rangeVariable->getType(), $compilationContext);
        }

        /**
         * Create a copy of the current value in the end of the range to avoid modify the range
         * inside the cycle
         */
        if ($parameters[1]->getType() != 'variable') {
            $upperBoundVariable = $compilationContext->symbolTable->getTempVariable($parameters[1]->getType(), $compilationContext);
        } else {
            $rangeVariable = $compilationContext->symbolTable->getVariableForRead($parameters[1]->getCode(), $compilationContext, $this->_statement['expr']);
            $upperBoundVariable = $compilationContext->symbolTable->getTempVariable($rangeVariable->getType(), $compilationContext);
        }

        /**
         * Create an implicit 'let' operation to set the current value in the upper bound of the range
         */
        $statement = new LetStatement(array(
            'type' => 'let',
            'assignments' => array(
                array(
                    'assign-type' => 'variable',
                    'variable' => $upperBoundVariable->getName(),
                    'operator' => 'assign',
                    'expr' => array(
                        'type' => $parameters[1]->getType(),
                        'value' => $parameters[1]->getCode(),
                        'file' => $this->_statement['file'],
                        'line' => $this->_statement['line'],
                        'char' => $this->_statement['char']
                    ),
                    'file' => $this->_statement['file'],
                    'line' => $this->_statement['line'],
                    'char' => $this->_statement['char']
                )
            )
        ));
        $statement->compile($compilationContext);

        if ($this->_statement['reverse']) {
            /**
             * Create an implicit 'let' operation for the initialize expression, @TODO use a builder
             */
            $statement = new LetStatement(array(
                'type' => 'let',
                'assignments' => array(
                    array(
                        'assign-type' => 'variable',
                        'variable' => $tempVariable->getName(),
                        'operator' => 'assign',
                        'expr' => array(
                            'type' => 'variable',
                            'value' => $upperBoundVariable->getName(),
                            'file' => $this->_statement['file'],
                            'line' => $this->_statement['line'],
                            'char' => $this->_statement['char']
                        ),
                        'file' => $this->_statement['file'],
                        'line' => $this->_statement['line'],
                        'char' => $this->_statement['char']
                    )
                )
            ));
        } else {
            /**
             * Create an implicit 'let' operation for the initialize expression, @TODO use a builder
             */
            $statement = new LetStatement(array(
                'type' => 'let',
                'assignments' => array(
                    array(
                        'assign-type' => 'variable',
                        'variable' => $tempVariable->getName(),
                        'operator' => 'assign',
                        'expr' => array(
                            'type'  => $parameters[0]->getType(),
                            'value' => $parameters[0]->getCode(),
                            'file'  => $this->_statement['file'],
                            'line'  => $this->_statement['line'],
                            'char'  => $this->_statement['char']
                        ),
                        'file' => $this->_statement['file'],
                        'line' => $this->_statement['line'],
                        'char' => $this->_statement['char']
                    )
                )
            ));
        }

        $statement->compile($compilationContext);

        /**
         * Initialize 'key' variable
         */
        if (isset($this->_statement['key'])) {
            $codePrinter->output($keyVariable->getName() . ' = 0;');
        }
        $codePrinter->output($flagVariable->getName() . ' = 0;');

        if ($this->_statement['reverse']) {
            $conditionExpr = array(
                'type' => 'greater-equal',
                'left' => array('type' => 'variable', 'value' => $tempVariable->getName()),
                'right' => array('type' => $parameters[0]->getType(), 'value' => $parameters[0]->getCode())
            );
        } else {
            $conditionExpr = array(
                'type' => 'less-equal',
                'left' => array('type' => 'variable', 'value' => $tempVariable->getName()),
                'right' => array('type' => 'variable', 'value' => $upperBoundVariable->getName())
            );
        }

        $expr = new EvalExpression();
        $condition = $expr->optimize($conditionExpr, $compilationContext);

        $codePrinter->output('if (' . $condition . ') {');
        $codePrinter->increaseLevel();

        /**
         * Inside a cycle
         */
        $compilationContext->insideCycle++;

        $codePrinter->output('while (1) {');
        $codePrinter->increaseLevel();

        $codePrinter->output('if (' . $flagVariable->getName() . ') {');

        $codePrinter->increaseLevel();

        if (isset($this->_statement['key'])) {
            $codePrinter->output($keyVariable->getName() . '++;');
        }

        if ($this->_statement['reverse']) {
            if (!isset($parameters[2])) {
                $statement = new LetStatement(array(
                    'type' => 'let',
                    'assignments' => array(
                        array(
                            'assign-type' => 'decr',
                            'variable' => $tempVariable->getName(),
                            'file' => $this->_statement['file'],
                            'line' => $this->_statement['line'],
                            'char' => $this->_statement['char']
                        )
                    )
                ));
            } else {
                $statement = new LetStatement(array(
                    'type' => 'let',
                    'assignments' => array(
                        array(
                            'assign-type' => 'variable',
                            'operator' => 'sub-assign',
                            'variable' => $tempVariable->getName(),
                            'expr' => array(
                                'type' => $parameters[2]->getType(),
                                'value' => $parameters[2]->getCode(),
                                'file' => $this->_statement['file'],
                                'line' => $this->_statement['line'],
                                'char' => $this->_statement['char']
                            ),
                            'file' => $this->_statement['file'],
                            'line' => $this->_statement['line'],
                            'char' => $this->_statement['char']
                        )
                    )
                ));
            }
        } else {
            if (!isset($parameters[2])) {
                $statement = new LetStatement(array(
                    'type' => 'let',
                    'assignments' => array(
                        array(
                            'assign-type' => 'incr',
                            'variable' => $tempVariable->getName(),
                            'file' => $this->_statement['file'],
                            'line' => $this->_statement['line'],
                            'char' => $this->_statement['char']
                        )
                    )
                ));
            } else {
                $statement = new LetStatement(array(
                    'type' => 'let',
                    'assignments' => array(
                        array(
                            'assign-type' => 'variable',
                            'operator' => 'add-assign',
                            'variable' => $tempVariable->getName(),
                            'expr' => array(
                                'type' => $parameters[2]->getType(),
                                'value' => $parameters[2]->getCode(),
                                'file' => $this->_statement['file'],
                                'line' => $this->_statement['line'],
                                'char' => $this->_statement['char']
                            ),
                            'file' => $this->_statement['file'],
                            'line' => $this->_statement['line'],
                            'char' => $this->_statement['char']
                        )
                    )
                ));
            }
        }
        $statement->compile($compilationContext);

        /**
         * Multi-line conditions would need to be regenerated here
         */
        $condition = $expr->optimize($conditionExpr, $compilationContext);
        $codePrinter->output('if (!(' . $condition . ')) {');
        $codePrinter->output("\t" . "break;");
        $codePrinter->output('}');

        $codePrinter->decreaseLevel();

        $codePrinter->output('} else {');
        $codePrinter->output("\t" . $flagVariable->getName() . ' = 1;');
        $codePrinter->output('}');

        /**
         * Initialize 'key' variable
         */
        if (isset($this->_statement['key'])) {
            /**
             * Check for anonymous variables
             */
            if ($this->_statement['key'] != '_') {
                $keyVariableName = $this->_statement['key'];
            } else {
                $keyVariableName = $keyVariable->getName();
            }

            /**
             * Create an implicit 'let' operation, @TODO use a builder
             */
            $statement = new LetStatement(array(
                'type' => 'let',
                'assignments' => array(
                    array(
                        'assign-type' => 'variable',
                        'variable' => $keyVariableName,
                        'operator' => 'assign',
                        'expr' => array(
                            'type' => 'variable',
                            'value' => $keyVariable->getName(),
                            'file' => $this->_statement['file'],
                            'line' => $this->_statement['line'],
                            'char' => $this->_statement['char']
                        ),
                        'file' => $this->_statement['file'],
                        'line' => $this->_statement['line'],
                        'char' => $this->_statement['char']
                    )
                )
            ));

            $statement->compile($compilationContext);
        }

        /**
         * Initialize 'value' variable
         */
        if (isset($this->_statement['value'])) {
            /**
             * Check for anonymous variables
             */
            if ($this->_statement['value'] != '_') {
                $valueVariable = $this->_statement['value'];
            } else {
                $valueVariable = $tempVariable->getName();
            }

            /**
             * Create an implicit 'let' operation, @TODO use a builder
             */
            $statement = new LetStatement(array(
                'type' => 'let',
                'assignments' => array(
                    array(
                        'assign-type' => 'variable',
                        'variable' => $valueVariable,
                        'operator' => 'assign',
                        'expr' => array(
                            'type'  => 'variable',
                            'value' => $tempVariable->getName(),
                            'file'  => $this->_statement['file'],
                            'line'  => $this->_statement['line'],
                            'char'  => $this->_statement['char']
                        ),
                        'file' => $this->_statement['file'],
                        'line' => $this->_statement['line'],
                        'char' => $this->_statement['char']
                    )
                )
            ));

            $statement->compile($compilationContext);
        }

        $codePrinter->decreaseLevel();

        /**
         * Compile statements in the 'for' block
         */
        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $st->isLoop(true);
            if (isset($this->_statement['key'])) {
                $st->getMutateGatherer()->increaseMutations($this->_statement['key']);
            }
            $st->getMutateGatherer()->increaseMutations($this->_statement['value']);
            $st->compile($compilationContext);
        }

        /**
         * Restore the cycle counter
         */
        $compilationContext->insideCycle--;

        $codePrinter->output('}');

        $codePrinter->decreaseLevel();

        $codePrinter->output('}');
    }

    /**
     * Compiles a 'for' statement that use an 'iterator' as expression
     *
     * @param array $exprRaw
     * @param \CompilationContext $compilationContext
     * @return boolean
     */
    public function compileIterator(array $exprRaw, CompilationContext $compilationContext)
    {
        $iteratorVariable = $compilationContext->symbolTable->getTempVariableForWrite('zend_object_iterator', $compilationContext);

        $compilationContext->headersManager->add('kernel/iterator');

        $codePrinter = $compilationContext->codePrinter;

        /**
         * Resolve first parameter of iterator(p)
         */
        $expr = new Expression($exprRaw['parameters'][0]['parameter']);
        $expr->setReadOnly(false);
        $expression = $expr->compile($compilationContext);

        if ($expression->getType() != 'variable') {
            throw new CompilerException("Unknown type: " . $expression->getType(), $exprRaw);
        }

        $exprVariable = $compilationContext->symbolTable->getVariableForRead($expression->getCode(), $compilationContext, $this->_statement['expr']);
        switch ($exprVariable->getType()) {
            case 'variable':
                break;
        }

        /**
         * Initialize 'key' variable
         */
        if (isset($this->_statement['key'])) {
            if ($this->_statement['key'] != '_') {
                $keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $compilationContext, $this->_statement['expr']);
                if ($keyVariable->getType() != 'variable') {
                    throw new CompilerException("Cannot use variable: " . $this->_statement['key'] . " type: " . $keyVariable->getType() . " as key in hash traversal", $this->_statement['expr']);
                }
            } else {
                /**
                 * Anonymous key variable
                 */
                $keyVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $keyVariable->increaseUses();
            }

            $keyVariable->setMustInitNull(true);
            $keyVariable->setIsInitialized(true, $compilationContext, $this->_statement);
            $keyVariable->setDynamicTypes('undefined');
        }

        /**
         * Initialize 'value' variable
         */
        if (isset($this->_statement['value'])) {
            if ($this->_statement['value'] != '_') {
                $variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $compilationContext, $this->_statement['expr']);
                if ($variable->getType() != 'variable') {
                    throw new CompilerException("Cannot use variable: " . $this->_statement['value'] . " type: " . $variable->getType() . " as value in hash traversal", $this->_statement['expr']);
                }
            } else {
                /**
                 * Anonymous value variable
                 */
                $variable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $variable->increaseUses();
            }

            $variable->setMustInitNull(true);
            $variable->setIsInitialized(true, $compilationContext, $this->_statement);
            $variable->setDynamicTypes('undefined');
        }

        /**
         * Variables are initialized in a different way inside cycle
         */
        $compilationContext->insideCycle++;

        $codePrinter->output($iteratorVariable ->getName() . ' = zephir_get_iterator(' . $exprVariable->getName() . ' TSRMLS_CC);');

        $codePrinter->output($iteratorVariable ->getName() . '->funcs->rewind(' . $iteratorVariable->getName() . ' TSRMLS_CC);');
        $codePrinter->output('for (;' . $iteratorVariable->getName() . '->funcs->valid(' . $iteratorVariable->getName() . ' TSRMLS_CC) == SUCCESS && !EG(exception); ' . $iteratorVariable ->getName() . '->funcs->move_forward(' . $iteratorVariable ->getName() . ' TSRMLS_CC)) {');

        if (isset($this->_statement['key'])) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->output("\t" . 'ZEPHIR_GET_IMKEY(' . $this->_statement['key'] . ', ' . $iteratorVariable->getName() . ');');
        }

        if (isset($this->_statement['value'])) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->output("\t" . '{');
            $codePrinter->output("\t\t" . 'zval **ZEPHIR_TMP_ITERATOR_PTR;');
            $codePrinter->output("\t\t" . $iteratorVariable->getName() . '->funcs->get_current_data(' . $iteratorVariable->getName() . ', &ZEPHIR_TMP_ITERATOR_PTR TSRMLS_CC);');
            $codePrinter->output("\t\t" . 'ZEPHIR_CPY_WRT(' . $variable->getName() . ', (*ZEPHIR_TMP_ITERATOR_PTR));');
            $codePrinter->output("\t" . '}');
        }

        /**
         * Compile statements in the 'for' block
         */
        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $st->isLoop(true);
            if (isset($this->_statement['key'])) {
                $st->getMutateGatherer()->increaseMutations($this->_statement['key']);
            }
            $st->getMutateGatherer()->increaseMutations($this->_statement['value']);
            $st->compile($compilationContext);
        }

        /**
         * Restore the cycle counter
         */
        $compilationContext->insideCycle--;

        $codePrinter->output('}');

        $codePrinter->output($iteratorVariable ->getName() . '->funcs->dtor(' . $iteratorVariable ->getName() . ' TSRMLS_CC);');
    }

    /**
     * Compiles traversing of string values
     * - Evaluated expression must be a string
     * - Every key must be an integer or compatible
     * - Every value must be a char/integer or compatible
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @param Variable $exprVariable
     */
    public function compileStringTraverse($expression, CompilationContext $compilationContext, $exprVariable)
    {
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Initialize 'key' variable
         */
        if (isset($this->_statement['key'])) {
            if ($this->_statement['key'] != '_') {
                $keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $compilationContext, $this->_statement['expr']);
                switch ($keyVariable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        break;
                    default:
                        throw new CompilerException("Cannot use variable: " . $this->_statement['key'] . " type: " . $keyVariable->getType() . " as key in string traversal", $this->_statement['expr']);
                }
            } else {
                $keyVariable = $compilationContext->symbolTable->getTempVariableForWrite('int', $compilationContext);
                $keyVariable->increaseUses();
            }

            $keyVariable->setMustInitNull(true);
            $keyVariable->setIsInitialized(true, $compilationContext, $this->_statement);
        }

        /**
         * Initialize 'value' variable
         */
        if (isset($this->_statement['value'])) {
            if ($this->_statement['value'] != '_') {
                $variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $compilationContext, $this->_statement['expr']);
                switch ($variable->getType()) {
                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                    case 'char':
                    case 'uchar':
                        break;
                    default:
                        throw new CompilerException("Cannot use variable: " . $this->_statement['value'] . " type: " . $variable->getType() . " as value in string traversal", $this->_statement['expr']);
                }
            } else {
                $variable = $compilationContext->symbolTable->getTempVariableForWrite('char', $compilationContext);
                $variable->increaseUses();
            }

            $variable->setMustInitNull(true);
            $variable->setIsInitialized(true, $compilationContext, $this->_statement);
        }

        $tempVariable = $compilationContext->symbolTable->addTemp('long', $compilationContext);

        /**
         * Create a temporary value to store the constant string
         */
        if ($expression->getType() == 'string') {
            $constantVariable = $compilationContext->symbolTable->getTempLocalVariableForWrite('variable', $compilationContext, $this->_statement);
            $codePrinter->output('ZVAL_STRING(&' . $constantVariable->getName() . ', "' . Utils::addSlashes($expression->getCode()) . '", 0);');
            $stringVariable = $constantVariable;
        } else {
            $stringVariable = $exprVariable;
        }

        if ($this->_statement['reverse']) {
            if ($stringVariable->isLocalOnly()) {
                $codePrinter->output('for (' . $tempVariable->getName() . ' = Z_STRLEN_P(&' . $stringVariable->getName() . '); ' . $tempVariable->getName() . ' >= 0; ' . $tempVariable->getName() . '--) {');
            } else {
                $codePrinter->output('for (' . $tempVariable->getName() . ' = Z_STRLEN_P(' . $stringVariable->getName() . '); ' . $tempVariable->getName() . ' >= 0; ' . $tempVariable->getName() . '--) {');
            }
        } else {
            if ($stringVariable->isLocalOnly()) {
                $codePrinter->output('for (' . $tempVariable->getName() . ' = 0; ' . $tempVariable->getName() . ' < Z_STRLEN_P(&' . $stringVariable->getName() . '); ' . $tempVariable->getName() . '++) {');
            } else {
                $codePrinter->output('for (' . $tempVariable->getName() . ' = 0; ' . $tempVariable->getName() . ' < Z_STRLEN_P(' . $stringVariable->getName() . '); ' . $tempVariable->getName() . '++) {');
            }
        }

        if (isset($this->_statement['key'])) {
            $codePrinter->output("\t" . $keyVariable->getName() . ' = ' . $tempVariable->getName() . '; ');
        }

        $compilationContext->headersManager->add('kernel/operators');
        if ($stringVariable->isLocalOnly()) {
            $codePrinter->output("\t" . $variable->getName() . ' = ZEPHIR_STRING_OFFSET(&' . $stringVariable->getName() . ', ' . $tempVariable->getName() . ');');
        } else {
            $codePrinter->output("\t" . $variable->getName() . ' = ZEPHIR_STRING_OFFSET(' . $stringVariable->getName() . ', ' . $tempVariable->getName() . ');');
        }

        /**
         * Variables are initialized in a different way inside cycle
         */
        $compilationContext->insideCycle++;

        /**
         * Compile statements in the 'for' block
         */
        if (isset($this->_statement['statements'])) {
            $st = new StatementsBlock($this->_statement['statements']);
            $st->isLoop(true);
            if (isset($this->_statement['key'])) {
                $st->getMutateGatherer()->increaseMutations($this->_statement['key']);
            }
            $st->getMutateGatherer()->increaseMutations($this->_statement['value']);
            $st->compile($compilationContext);
        }

        $compilationContext->insideCycle--;

        $codePrinter->output('}');
    }

    /**
     * Compiles traversing of hash values
     *
     * - Evaluated expression must be a zval
     * - A key must be a zval
     * - A value must be a zval
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @param Variable $exprVariable
     */
    public function compileHashTraverse($expression, CompilationContext $compilationContext, Variable $exprVariable)
    {
        $codePrinter = $compilationContext->codePrinter;

        /**
         * Initialize 'key' variable
         */
        if (isset($this->_statement['key'])) {
            if ($this->_statement['key'] != '_') {
                $keyVariable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['key'], $compilationContext, $this->_statement['expr']);
                if ($keyVariable->getType() != 'variable') {
                    throw new CompilerException("Cannot use variable: " . $this->_statement['key'] . " type: " . $keyVariable->getType() . " as key in hash traversal", $this->_statement['expr']);
                }
            } else {
                $keyVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
            }

            $keyVariable->setMustInitNull(true);
            $keyVariable->setIsInitialized(true, $compilationContext, $this->_statement);
            $keyVariable->setDynamicTypes('undefined');
        }

        /**
         * Initialize 'value' variable
         */
        if (isset($this->_statement['value'])) {
            if ($this->_statement['value'] != '_') {
                $variable = $compilationContext->symbolTable->getVariableForWrite($this->_statement['value'], $compilationContext, $this->_statement['expr']);
                if ($variable->getType() != 'variable') {
                    throw new CompilerException("Cannot use variable: " . $this->_statement['value'] . " type: " . $variable->getType() . " as value in hash traversal", $this->_statement['expr']);
                }
            } else {
                $variable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
            }

            $variable->setMustInitNull(true);
            $variable->setIsInitialized(true, $compilationContext, $this->_statement);
            $variable->setDynamicTypes('undefined');
        }

        /**
         * Variables are initialized in a different way inside cycle
         */
        $compilationContext->insideCycle++;

        /**
         * Create a hash table and hash pointer temporary variables
         */
        $arrayPointer = $compilationContext->symbolTable->addTemp('HashPosition', $compilationContext);
        $arrayHash = $compilationContext->symbolTable->addTemp('HashTable', $compilationContext);

        /**
         * Create a temporary zval to fetch the items from the hash
         */
        $tempVariable = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $tempVariable->setIsDoublePointer(true);

        $compilationContext->headersManager->add('kernel/hash');

        $duplicateHash = '0';
        $duplicateKey = true;

        /**
         * We have to check if hashes are modified within the for's block
         */
        if (isset($this->_statement['statements'])) {
            /**
             * Create the statements block here to obtain the last use line
             */
            $st = new StatementsBlock($this->_statement['statements']);

            $detector = new ForValueUseDetector();
            if ($detector->detect($exprVariable->getName(), $this->_statement['statements'])) {
                $duplicateHash = '1';
            }

            /**
             * Detect if the key is modified or passed to an external scope
             */
            if (isset($this->_statement['key'])) {
                if (!$keyVariable->isTemporal()) {
                    $detector->setDetectionFlags(ForValueUseDetector::DETECT_ALL);
                    if ($detector->detect($keyVariable->getName(), $this->_statement['statements'])) {
                        $loopContext = $compilationContext->currentMethod->getLocalContextPass();
                        //echo $st->getLastLine();
                        //echo $loopContext->getLastVariableUseLine($keyVariable->getName());
                        $duplicateKey = true;
                    }
                }
            }
        }

        $codePrinter->output('zephir_is_iterable(' . $expression->getCode() . ', &' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ', ' . $duplicateHash . ', ' . $this->_statement['reverse'] . ', "' . Compiler::getShortUserPath($this->_statement['file']) . '", ' . $this->_statement['line'] . ');');

        $codePrinter->output('for (');
        $codePrinter->output('  ; zephir_hash_get_current_data_ex(' . $arrayHash->getName() . ', (void**) &' . $tempVariable->getName() . ', &' . $arrayPointer ->getName() . ') == SUCCESS');
        if ($this->_statement['reverse']) {
            $codePrinter->output('  ; zephir_hash_move_backwards_ex(' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ')');
        } else {
            $codePrinter->output('  ; zephir_hash_move_forward_ex(' . $arrayHash->getName() . ', &' . $arrayPointer ->getName() . ')');
        }
        $codePrinter->output(') {');

        if (isset($this->_statement['key'])) {
            if ($duplicateKey) {
                $compilationContext->symbolTable->mustGrownStack(true);
                $codePrinter->output("\t" . 'ZEPHIR_GET_HMKEY(' . $keyVariable->getName() . ', ' . $arrayHash->getName() . ', ' . $arrayPointer ->getName() . ');');
            } else {
                $codePrinter->output("\t" . 'ZEPHIR_GET_HKEY(' . $keyVariable->getName() . ', ' . $arrayHash->getName() . ', ' . $arrayPointer ->getName() . ');');
            }
        }

        if (isset($this->_statement['value'])) {
            $compilationContext->symbolTable->mustGrownStack(true);
            $codePrinter->output("\t" . 'ZEPHIR_GET_HVALUE(' . $variable->getName() . ', ' . $tempVariable->getName() . ');');
        }

        /**
         * Compile statements in the 'for' block
         */
        if (isset($this->_statement['statements'])) {
            $st->isLoop(true);
            if (isset($this->_statement['key'])) {
                $st->getMutateGatherer()->increaseMutations($this->_statement['key']);
            }
            $st->getMutateGatherer()->increaseMutations($this->_statement['value']);
            $st->compile($compilationContext);
        }

        /**
         * Restore the cycle counter
         */
        $compilationContext->insideCycle--;

        $codePrinter->output('}');
    }

    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $exprRaw = $this->_statement['expr'];

        /**
         * @TODO implement optimizers here
         */
        if ($exprRaw['type'] == 'fcall') {
            if ($exprRaw['name'] == 'range') {
                $status = $this->compileRange($exprRaw, $compilationContext);
                if ($status !== false) {
                    return;
                }
            }

            if ($exprRaw['name'] == 'iterator') {
                $status = $this->compileIterator($exprRaw, $compilationContext);
                if ($status !== false) {
                    return;
                }
            }
        }

        $expr = new Expression($exprRaw);
        $expr->setReadOnly(true);
        $expression = $expr->compile($compilationContext);

        /**
         * Check for traversing a constant string
         */
        if ($expression->getType() == 'string') {
            $this->compileStringTraverse($expression, $compilationContext, null);
            return;
        }

        if ($expression->getType() != 'variable' && $expression->getType() != 'array') {
            throw new CompilerException("Unknown type: " . $expression->getType(), $exprRaw);
        }

        $exprVariable = $compilationContext->symbolTable->getVariableForRead($expression->getCode(), $compilationContext, $this->_statement['expr']);
        switch ($exprVariable->getType()) {

            case 'variable':
            case 'array':
                $this->compileHashTraverse($expression, $compilationContext, $exprVariable);
                break;

            case 'string':
                $this->compileStringTraverse($expression, $compilationContext, $exprVariable);
                break;

            default:
                throw new CompilerException("Cannot traverse value type: " . $exprVariable->getType(), $exprRaw);
        }
    }
}

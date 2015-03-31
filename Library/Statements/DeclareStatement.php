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

use Zephir\CompilationContext;
use Zephir\CompilerException;
use Zephir\Expression;
use Zephir\LiteralCompiledExpression;

/**
 * DeclareStatement
 *
 * This creates variables in the current symbol table
 */
class DeclareStatement extends StatementAbstract
{
    /**
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        $statement = $this->_statement;

        if (!isset($statement['data-type'])) {
            throw new CompilerException("Data type is required", $this->_statement);
        }

        $typeInference = $compilationContext->typeInference;
        $symbolTable = $compilationContext->symbolTable;

        foreach ($statement['variables'] as $variable) {
            if ($symbolTable->hasVariable($variable['variable'])) {
                throw new CompilerException("Variable '" . $variable['variable'] . "' is already defined", $variable);
            }

            $currentType = $statement['data-type'];

            /**
             * Replace original data type by the pre-processed infered type
             */
            if ($typeInference) {
                if ($currentType == 'variable') {
                    $type = $typeInference->getInferedType($variable['variable']);
                    if (is_string($type)) {
                        $currentType = $type;
                    }
                }
            }

            /**
             * Variables are added to the symbol table
             */
            if (isset($variable['expr'])) {
                $symbolVariable = $symbolTable->addVariable($currentType, $variable['variable'], $compilationContext, $variable['expr']);
            } else {
                $symbolVariable = $symbolTable->addVariable($currentType, $variable['variable'], $compilationContext);
            }

            /**
             * Set the node where the variable is declared
             */
            $symbolVariable->setOriginal($variable);

            if (isset($variable['expr']['type'])) {
                $defaultType = $variable['expr']['type'];
            } else {
                $defaultType = null;
            }

            if (isset($variable['expr']['value'])) {
                $defaultValue = $variable['expr']['value'];
            } else {
                $defaultValue = null;
            }

            /**
             * Variables with a default value are initialized by default
             */
            if ($defaultValue !== null || $defaultType !== null) {
                if ($currentType == 'variable' || $currentType == 'string' || $currentType == 'array') {
                    $symbolVariable->increaseVariantIfNull();
                }

                switch ($currentType) {

                    case 'int':
                    case 'uint':
                    case 'ulong':
                    case 'long':
                        switch ($defaultType) {

                            case 'int':
                            case 'uint':
                            case 'ulong':
                                break;

                            case 'null':
                                $defaultValue = 0;
                                break;

                            default:
                                self::invalidDefaultTypeException($variable['expr']['type'], $statement['data-type'], $variable);
                        }
                        break;

                    case 'double':
                        switch ($defaultType) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'double':
                                break;

                            case 'null':
                                $defaultValue = 0;
                                break;

                            default:
                                self::invalidDefaultTypeException($variable['expr']['type'], $statement['data-type'], $variable);
                        }
                        break;

                    case 'bool':
                        switch ($defaultType) {

                            case 'bool':
                                if ($variable['expr']['value'] == 'true') {
                                    $defaultValue = 1;
                                } else {
                                    $defaultValue = 0;
                                }
                                break;

                            case 'null':
                                $defaultValue = 0;
                                break;

                            default:
                                self::invalidDefaultTypeException($variable['expr']['type'], $statement['data-type'], $variable);
                        }
                        break;

                    case 'char':
                    case 'uchar':
                        switch ($defaultType) {

                            case 'char':
                            case 'uchar':
                                $defaultValue = '\'' . $defaultValue . '\'';
                                break;

                            case 'int':
                                break;

                            case 'null':
                                $defaultValue = 0;
                                break;

                            default:
                                self::invalidDefaultTypeException($variable['expr']['type'], $statement['data-type'], $variable);
                        }
                        break;

                    case 'string':
                        $defaultValue = $variable['expr'];
                        switch ($defaultType) {

                            case 'string':
                            case 'null':
                                break;

                            default:
                                self::invalidDefaultTypeException($variable['expr']['type'], $statement['data-type'], $variable);
                        }
                        break;

                    case 'array':
                        $defaultValue = $variable['expr'];
                        switch ($defaultType) {

                            case 'null':
                            case 'array':
                            case 'empty-array':
                                break;

                            default:
                                self::invalidDefaultTypeException($variable['expr']['type'], $statement['data-type'], $variable);
                        }
                        break;

                    case 'variable':
                        $defaultValue = $variable['expr'];
                        switch ($defaultType) {

                            case 'int':
                            case 'uint':
                            case 'long':
                            case 'char':
                            case 'uchar':
                                $symbolVariable->setDynamicTypes('long');
                                break;

                            case 'double':
                                $symbolVariable->setDynamicTypes('double');
                                break;

                            case 'string':
                            case 'ulong':
                                $symbolVariable->setDynamicTypes('string');
                                break;

                            case 'array':
                                $expression = new Expression($variable['expr']);
                                $expression->setExpectReturn(true, $symbolVariable);
                                $expression->compile($compilationContext);
                                // no break

                            case 'array':
                            case 'empty-array':
                                $symbolVariable->setDynamicTypes('array');
                                break;

                            case 'null':
                                $symbolVariable->setDynamicTypes('null');
                                $symbolVariable->setMustInitNull(true);
                                $symbolVariable->setLocalOnly(false);
                                break;

                            default:
                                self::invalidDefaultTypeException($defaultType, $statement['data-type'], $variable);
                        }
                        break;

                    default:
                        throw new CompilerException('Invalid variable type: ' . $currentType, $variable);
                }

                $symbolVariable->setDefaultInitValue($defaultValue);
                $symbolVariable->setIsInitialized(true, $compilationContext);
                $symbolVariable->increaseMutates();
                $symbolVariable->setPossibleValue(new LiteralCompiledExpression($defaultType, $defaultValue, $variable['expr']), $compilationContext);
            }
        }
    }

    /**
     * throw exception for invalid default type
     * @param $defaultType
     * @param $dateType
     * @param $variable
     * @throws CompilerException
     */
    public static function invalidDefaultTypeException($defaultType, $dateType, $variable)
    {
        throw new CompilerException('Invalid default type: ' . $defaultType . ' for data type: ' . $dateType, $variable);
    }
}

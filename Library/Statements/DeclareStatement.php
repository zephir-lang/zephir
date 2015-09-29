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
use Zephir\Expression\Builder\BuilderFactory;
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

            switch ($currentType) {
                case 'variable':
                case 'array-access':
                case 'property-access':
                case 'static-property-access':
                case 'fcall':
                case 'mcall':
                case 'scall':
                    $currentType = 'variable';
                    break;
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

            $symbolVariable->setIsInitialized(true, $compilationContext);
            //$symbolVariable->increaseMutates();

            if ($currentType == 'variable') {
                $symbolVariable->setMustInitNull(true);
                $symbolVariable->setLocalOnly(false);
            }
            //$symbolVariable->increaseVariantIfNull();

            if (isset($variable['expr'])) {
                $builder = BuilderFactory::getInstance();
                $letBuilder = $builder->statements()->let(array(
                    $builder->operators()
                        ->assignVariable($variable['variable'], $builder->raw($variable['expr']))
                ));

                $letStatement = new LetStatement($letBuilder->build());
                $letStatement->compile($compilationContext);
            } else {
                $symbolVariable->enableDefaultAutoInitValue();
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

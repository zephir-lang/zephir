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

namespace Zephir\Traits;

use Zephir\CompilationContext;
use Zephir\Statements\LetStatement;
use Zephir\Types\Types;
use Zephir\Variable\Variable;

trait VarOptimizerTrait
{
    /**
     * @param Variable|null $symbolVariable
     * @param mixed                          $resolvedParam
     * @param CompilationContext             $context
     * @param array                          $expression
     *
     * @return bool|Variable
     */
    private function processVariable(
        ?Variable $symbolVariable,
        mixed $resolvedParam,
        CompilationContext $context,
        array $expression
    ): bool | Variable {
        if (!$symbolVariable || !$symbolVariable->isVariable()) {
            /*
             * Complex expressions require a temporary variable
             */
            $type = match ($resolvedParam->getType()) {
                Types::T_ARRAY => 'array',
                default        => 'variable',
            };

            $variable = $context->symbolTable->addTemp($type, $context);
            $variable->initVariant($context);

            $statement = new LetStatement([
                'type'        => 'let',
                'assignments' => [
                    [
                        'assign-type' => $type,
                        'variable'    => $variable->getName(),
                        'operator'    => 'assign',
                        'expr'        => [
                            'type'  => $resolvedParam->getType(),
                            'value' => $resolvedParam->getCode(),
                            'file'  => $expression['file'],
                            'line'  => $expression['line'],
                            'char'  => $expression['char'],
                        ],
                        'file'        => $expression['file'],
                        'line'        => $expression['line'],
                        'char'        => $expression['char'],
                    ],
                ],
            ]);
            $statement->compile($context);
        } else {
            /**
             * This mark the variable as used.
             */
            $variable = $context->symbolTable->getVariableForRead(
                $resolvedParam->getCode(),
                $context,
                $expression
            );
        }

        return $variable;
    }
}

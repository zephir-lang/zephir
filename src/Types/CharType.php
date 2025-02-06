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

namespace Zephir\Types;

use ReflectionException;
use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;

class CharType extends AbstractType
{
    public function getTypeName(): string
    {
        return Types::T_CHAR;
    }

    /**
     * Transforms calls to method "toHex" to sprintf('%X') call.
     *
     * @param object             $caller
     * @param CompilationContext $compilationContext
     * @param Call               $call
     * @param array              $expression
     *
     * @return bool|mixed|CompiledExpression
     *
     * @throws ReflectionException
     * @throws Exception
     */
    public function toHex($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $exprBuilder  = BuilderFactory::getInstance();
        $functionCall = $exprBuilder->statements()
                                    ->functionCall('zephir_string_to_hex', [$caller])
                                    ->setFile($expression['file'])
                                    ->setLine($expression['line'])
                                    ->setChar($expression['char'])
        ;

        $expression = new Expression($functionCall->build());

        return $expression->compile($compilationContext);
    }
}

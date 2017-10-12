<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Types;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;

class CharType extends AbstractType
{
    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return 'char';
    }

    /**
     * Transforms calls to method "toHex" to sprintf('%X') call
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     * @return bool|mixed|\Zephir\CompiledExpression
     */
    public function toHex($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $exprBuilder = BuilderFactory::getInstance();
        $functionCall = $exprBuilder->statements()
            ->functionCall('zephir_string_to_hex', array($caller))
            ->setFile($expression['file'])
            ->setLine($expression['line'])
            ->setChar($expression['char']);

        $expression = new Expression($functionCall->build());

        return $expression->compile($compilationContext);
    }
}

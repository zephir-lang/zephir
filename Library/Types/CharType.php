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

namespace Zephir\Types;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\FunctionCall;
use Zephir\Builder\FunctionCallBuilder;
use Zephir\Types;

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
            ->functionCall('sprintf', array($exprBuilder->literal(Types::STRING, '%X'), $caller))
            ->setFile($expression['file'])
            ->setLine($expression['line'])
            ->setChar($expression['char']);

        $expression = new Expression($functionCall->build());

        return $expression->compile($compilationContext);
    }
}

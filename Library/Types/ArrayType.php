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
use Zephir\Builder\FunctionCallBuilder;
use Zephir\FunctionCall;

/**
 * ArrayType
 *
 * Defines methods of the built-in array type
 */
class ArrayType extends AbstractType
{
    /**
     * {@inheritdoc}
     */
    public $methodMap = array(
        'join' => 'join',
        'reversed' => 'array_reverse',
        'rev' => 'array_reverse',
        'diff' => 'array_diff',
        'flip' => 'array_flip',
        'fill' => 'array_fill',
        'walk' => 'array_walk',
        'haskey' => 'array_key_exists',
        'keys' => 'array_keys',
        'values' => 'array_values',
        'split' => 'array_chunk',
        'combine' => 'array_combine',
        'intersect' => 'array_intersect',
        'merge' => 'array_merge',
        'mergerecursive' => 'array_merge_recursive',
        'pad' => 'array_pad',
        'pop' => 'array_pop',
        'push' => 'array_push',
        'rand' => 'array_rand',
        'replace' => 'array_replace',
        'map' => 'array_map',
        'replacerecursive' => 'array_replace_recursive',
        'shift' => 'array_shift',
        'slice' => 'array_slice',
        'splice' => 'array_splice',
        'sum' => 'array_sum',
        'unique' => 'array_unique',
        'prepend' => 'array_unshift',
        'count' => 'count',
        'current' => 'current',
        'each' => 'each',
        'end' => 'end',
        'key' => 'key',
        'next' => 'next',
        'prev' => 'prev',
        'reset' => 'reset',
        'sort' => 'sort',
        'sortbykey' => 'ksort',
        'reversesort' => 'rsort',
        'reversesortbykey' => 'krsort',
        'shuffle' => 'shuffle',
        'tojson' => 'json_encode',
        'reduce' => 'array_reduce',
    );

    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return 'array';
    }

    /**
     * {@inheritdoc}
     */
    protected function getNumberParam($methodName)
    {
        if ($methodName == "map") {
            return 1;
        }

        return 0;
    }

    /**
     * Transforms calls to method "join" to function calls to "join"
     *
     * @param object $caller
     * @param CompilationContext $compilationContext
     * @param Call $call
     * @param array $expression
     * @return bool|\Zephir\CompiledExpression
     */
    public function join($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $builder = new FunctionCallBuilder(
            'join',
            array_merge($expression['parameters'], array(array('parameter' => $caller))),
            FunctionCall::CALL_NORMAL,
            $expression['file'],
            $expression['line'],
            $expression['char']
        );

        $expression = new Expression($builder->get());

        return $expression->compile($compilationContext);
    }
}

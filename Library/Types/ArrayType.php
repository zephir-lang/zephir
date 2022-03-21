<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Types;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Types;

/**
 * Defines methods of the built-in array type
 */
class ArrayType extends AbstractType
{
    /**
     * {@inheritdoc}
     */
    public $methodMap = [
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
    ];

    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return Types::T_ARRAY;
    }

    /**
     * Transforms calls to method "join" to function calls to "join".
     *
     * @param object             $caller
     * @param CompilationContext $compilationContext
     * @param Call               $call
     * @param array              $expression
     *
     * @return bool|\Zephir\CompiledExpression
     */
    public function join($caller, CompilationContext $compilationContext, Call $call, array $expression)
    {
        $functionCall = BuilderFactory::getInstance()->statements()
            ->functionCall('join', $expression['parameters'])
            ->addArgument($caller)
            ->setFile($expression['file'])
            ->setLine($expression['line'])
            ->setChar($expression['char']);

        $expression = new Expression($functionCall->build());

        return $expression->compile($compilationContext);
    }

    /**
     * {@inheritdoc}
     */
    protected function getNumberParam($methodName)
    {
        if ('map' == $methodName) {
            return 1;
        }

        return 0;
    }
}

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

/**
 * IntType
 *
 * Defines methods of the built-in int type
 */
class IntType extends AbstractType
{
    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return 'int';
    }

    /**
     * {@inheritdoc}
     */
    protected function getMethodMap()
    {
        return array(
            'abs' => 'abs',
            'tobinary' => 'decbin',
            'tohex' => 'dechex',
            'tooctal' => 'decoct',
            'pow' => 'pow',
            'sqrt' => 'sqrt',
            'exp' => 'exp',
            'sin' => 'sin',
            'cos' => 'cos',
            'tan' => 'tan',
            'asin' => 'asin',
            'acos' => 'acos',
            'atan' => 'atan',
            'log' => 'log'
        );
    }

    /**
     * Returns the number of the parameter where the object must be bound
     *
     * @param $methodName
     * @return int
     */
    protected function getNumberParam($methodName)
    {
        return 0;
    }
}

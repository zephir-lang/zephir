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
    public $methodMap = array(
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

    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return 'int';
    }
}

<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
use Zephir\CompilerException;
use Zephir\Builder\FunctionCallBuilder;

/**
 * StringType
 *
 * Encapsulates built-in methods for the "string" type
 */
class StringType extends AbstractType
{
    /**
     * {@inheritdoc}
     */
    public function getTypeName()
    {
        return 'string';
    }

    /**
     * {@inheritdoc}
     */
    protected function getMethodMap()
    {
        return array(
            'index' => 'strpos',
            'trim' => 'trim',
            'length' => 'strlen',
            'lower' => 'strtolower',
            'upper' => 'strtoupper'
        );
    }
}

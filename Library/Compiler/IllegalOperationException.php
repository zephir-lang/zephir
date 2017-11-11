<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: https://zephir-lang.com/license.html               |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Compiler;

use Zephir\TypeAwareInterface;

/**
 * Zephir\Compiler\IllegalOperationException
 *
 * @package Zephir\Compiler
 */
class IllegalOperationException extends CompilerException
{
    /**
     * IllegalOperationException constructor.
     *
     * @param array              $statement The statement
     * @param TypeAwareInterface $type      Oprator type
     * @param array|null         $extra     Extra info [optional].
     */
    public function __construct(array $statement, TypeAwareInterface $type, array $extra = null)
    {
        $message = sprintf(
            "Operator '%s' is not supported for variable type: %s",
            $statement['operator'],
            $type->getType()
        );

        parent::__construct($message, $extra ?: $statement);
    }
}

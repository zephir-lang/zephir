<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Exception;

use Zephir\TypeAwareInterface;

/**
 * Zephir\Exception\IllegalOperationException.
 */
class IllegalOperationException extends CompilerException
{
    /**
     * IllegalOperationException constructor.
     *
     * @param array              $statement The statement
     * @param TypeAwareInterface $type      Operator type
     * @param array|null         $extra     extra info [optional]
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

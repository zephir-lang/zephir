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

/**
 * Zephir\Exception\LogicException.
 *
 * Exception that represents error in the program logic.
 * This kind of exceptions should directly lead to a fix in your code.
 */
class LogicException extends \LogicException implements ExceptionInterface
{
    use ExceptionExtraAwareTrait;
}

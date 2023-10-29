<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Exception;

/**
 * Exception that represents error in the program logic.
 * This kind of exception should directly lead to a fix in your code.
 */
class LogicException extends \LogicException implements ExceptionInterface
{
    use ExceptionExtraAwareTrait;
}

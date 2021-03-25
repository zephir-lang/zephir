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
 * Zephir\Exception\InvalidArgumentException.
 */
class InvalidArgumentException extends \InvalidArgumentException implements ExceptionInterface
{
    use ExceptionExtraAwareTrait;
}

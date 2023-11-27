<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Exception;

/**
 * Exception thrown if an error which can only be found on runtime occurs.
 */
class RuntimeException extends \RuntimeException implements ExceptionInterface
{
    use ExceptionExtraAwareTrait;
}

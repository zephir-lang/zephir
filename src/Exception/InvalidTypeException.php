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

use function sprintf;

final class InvalidTypeException extends CompilerException
{
    /**
     * InvalidTypeException constructor.
     *
     * @param string     $type
     * @param array|null $expression
     */
    public function __construct(string $type, array $expression = null)
    {
        $message = sprintf(
            'Returning type: %s but this type is not compatible with return-type hints declared in the method',
            $type
        );

        parent::__construct($message, $expression);
    }
}

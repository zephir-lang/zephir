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
 * Zephir\Exception\ExceptionInterface.
 */
interface ExceptionInterface
{
    /**
     * Returns the code block in which the error occurred.
     *
     * @return string
     */
    public function getErrorRegion(): string;

    /**
     * Gets extra info.
     *
     * @return array|null
     */
    public function getExtra(): array | null;
}

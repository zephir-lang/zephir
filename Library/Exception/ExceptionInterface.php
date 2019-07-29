<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Exception;

/**
 * Zephir\Exception\ExceptionInterface.
 */
interface ExceptionInterface
{
    /**
     * Gets extra info.
     *
     * @return array
     */
    public function getExtra();

    /**
     * Returns the code block in which the error occurred.
     *
     * @return string
     */
    public function getErrorRegion();
}

<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Exception;

/**
 * Zephir\Exception\ExceptionInterface
 *
 * @package Zephir\Exception
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

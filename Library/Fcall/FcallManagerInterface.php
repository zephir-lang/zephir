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

namespace Zephir\Fcall;

/**
 * Zephir\Fcall\FcallManagerInterface
 *
 * @package Zephir\Fcall
 */
interface FcallManagerInterface
{
    public function macroIsRequired($macro);

    /**
     * Resolve internal fcall attributes to a suitable macro and ensure that it's generated during compilation.
     *
     * @param bool $static
     * @param int $doReturn tri-state: 0 -> no return value, 1 -> do return, 2 -> do return to given variable
     * @param int $paramCount
     * @return string
     */
    public function getMacro($static, $doReturn, $paramCount);

    public function genFcallCode();
}

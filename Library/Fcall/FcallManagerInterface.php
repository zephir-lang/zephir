<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Fcall;

/**
 * Zephir\Fcall\FcallManagerInterface.
 */
interface FcallManagerInterface
{
    public function macroIsRequired($macro);

    /**
     * Resolve internal fcall attributes to a suitable macro and ensure that it's generated during compilation.
     *
     * @param bool $static
     * @param int  $doReturn   tri-state: 0 -> no return value, 1 -> do return, 2 -> do return to given variable
     * @param int  $paramCount
     *
     * @return string
     */
    public function getMacro($static, $doReturn, $paramCount);

    public function genFcallCode();
}

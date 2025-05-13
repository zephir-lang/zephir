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

namespace Zephir\Backend;

interface FcallManagerInterface
{
    public function genFcallCode(): void;

    /**
     * Resolve internal fcall attributes to a suitable macro and ensure that it's generated during compilation.
     */
    public function getMacro(bool $static, int $doReturn, int $paramCount): string;
}

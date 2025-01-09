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

namespace Zephir\Console;

use Zephir\Zephir;

final class HelpCommand
{
    public static function execute(): void
    {
        $version = explode('-', Zephir::VERSION);

        echo Zephir::LOGO . sprintf('Zephir %s by the Phalcon Team', $version[0]) . PHP_EOL;
    }
}

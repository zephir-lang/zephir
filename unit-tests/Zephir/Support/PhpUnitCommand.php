<?php

/*
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Support;

use PHPUnit\TextUI\Command;

class PhpUnitCommand extends Command
{
    /**
     * Start testing.
     *
     * @return int
     */
    public static function start()
    {
        $exit = true;

        if (\getenv('PHPUNIT_DONT_EXIT')) {
            $exit = false;
        } elseif (defined('PHPUNIT_DONT_EXIT')) {
            $exit = (bool) constant('PHPUNIT_DONT_EXIT');
        }

        return parent::main($exit);
    }
}

<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

final class ExtensionInfoTest extends TestCase
{
    /**
     * @see config.json for Directive keys
     *
     * @return array
     */
    public function directiveProvider(): array
    {
        return [
            ['Test Extension => enabled', true],
            ['Test Extension support => Value', true],
            ['Test variable => Value', true],
            ['extension.test_ini_variable => On => On', true],
            ['ini-entry.my_setting_1', true],
            ['stub.db.my_setting_1', true],
            ['stub.orm.cache_enable', true],
            // Every scalar type carries a directive, not just bool and string
            // (issue #2449). A numeric one displays the raw ini string, a bool
            // one displays On/Off.
            ['stub.my_setting_2 => 10 => 10', true],
            ['stub.my_setting_3 => 15.2 => 15.2', true],
            ['stub.my_setting_4 => A => A', true],
            ['stub.my_setting_5 => custom_value => custom_value', true],
            ['stub.my_setting_6 => 64 => 64', true],
            ['stub.db.my_setting_2 => 100 => 100', true],
            ['stub.db.my_setting_3 => 7.5 => 7.5', true],
            ['stub.orm.cache_level => 3 => 3', true],
            ['stub.module_setting => On => On', true],
            ['stub.test.my_setting_1', false],
            ['stub.test.test_setting_1', false],
            ['stub.test.', false],
        ];
    }

    /**
     * @dataProvider directiveProvider()
     *
     * @param string $var
     * @param bool   $contains
     */
    public function testShouldBeWithoutDuplicatesNamespace(string $var, bool $contains): void
    {
        ob_start();

        phpinfo(INFO_MODULES);
        $phpinfo = ob_get_contents();

        ob_end_clean();

        if ($contains) {
            $this->assertStringContainsString($var, $phpinfo);
        } else {
            $this->assertStringNotContainsString($var, $phpinfo);
        }
    }
}

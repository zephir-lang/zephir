<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class ExtensionInfoTest extends TestCase
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

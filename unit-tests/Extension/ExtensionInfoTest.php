<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
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
     */
    public function directiveProvider(): array
    {
        return [
            ['Zephir Test Extension => enabled', true],
            ['Zephir test variable => Value', true],
            ['zephir.superpower', true],
            ['test.orm.cache_enable', true],
            ['test.my_setting_1', true],
            ['test.test.my_setting_1', false],
        ];
    }

    /**
     * @dataProvider directiveProvider()
     * @test
     */
    public function shouldBeWithoutDuplicatesNamespace(string $var, bool $contains)
    {
        ob_start();
        phpinfo(INFO_MODULES);
        $phpinfo = ob_get_contents();
        ob_end_clean() ;

        if ($contains) {
            $this->assertContains($var, $phpinfo);
        } else {
            $this->assertNotContains($var, $phpinfo);
        }
    }
}
